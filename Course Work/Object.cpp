#include "Object.h"
#include "imgui/imgui.h"

Object::Object()
{
}

Object::Object(std::string path, Shader* shader, glm::vec3 scale, float angle, glm::vec3 rot )
{
	bool loadout = Loader.LoadFile(path);

	for (int i = 0; i < Loader.LoadedMeshes.size(); i++)
	{
		objl::Mesh curMesh = Loader.LoadedMeshes[i];
		for (int j = 0; j < curMesh.Vertices.size(); j++) {
			vertex.push_back(curMesh.Vertices[j]);
		}
	}
	object_scale = scale;
	
	indexes = Loader.LoadedIndices.size();
	for (int i = 0; i < Loader.LoadedIndices.size(); i++) {
		LoadedIndices.push_back(Loader.LoadedIndices[i]);
	}
	objectShader = shader;
	object_angle = angle;
	object_rot = rot;
	lightning=glm::vec3(1.2f, 1.0f, 2.0f);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	// load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// A great thing about structs is that their memory layout is sequential for all its items.
	// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	// again translates to 3/2 floats which translates to a byte array.
	glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(objl::Vertex), &vertex[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes * sizeof(unsigned int), &LoadedIndices[0], GL_STATIC_DRAW);

	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(objl::Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(objl::Vertex), (void*)(3 * sizeof(float)));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(objl::Vertex), (void*)(6 * sizeof(float)));
	// vertex tangent

	glBindVertexArray(0);

}

Object::~Object()
{
}

void Object::GUI()
{

	ImGui::SliderFloat("light.x", &lightning.r, 0.0f, 100.0f);
	ImGui::SliderFloat("light.y", &lightning.g, 0.0f, 100.0f);
	ImGui::SliderFloat("light.z", &lightning.b, 0.0f, 100.0f);

}

void Object::Draw(glm::mat4 model, glm::mat4 m_Proj,glm::mat4 m_View, glm::vec3 camerapos, glm::vec4 color,unsigned int text)
{
	objectShader->use();
	model = glm::scale(model, object_scale);
	model = glm::rotate(model, glm::radians(object_angle), object_rot);


	objectShader->setVec3("light.position",lightning );


	// light properties
	objectShader->setVec3("light.ambient", glm::vec3(1.0f, 1.0f, 1.0f)); // note that all light colors are set at full intensity
	objectShader->setVec3("light.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
	objectShader->setVec3("light.specular", glm::vec3( 1.0f, 1.0f, 1.0f));

	// material properties
	objectShader->setVec3("material.ambient", glm::vec3(0.0f, 0.1f, 0.06f));
	objectShader->setVec3("material.diffuse", glm::vec3(0.0f, 0.50980392f, 0.50980392f));
	objectShader->setVec3("material.specular", glm::vec3(0.50196078f, 0.50196078f, 0.50196078f));
	objectShader->setFloat("material.shininess", 32.0f);
	objectShader->setVec3("viewPos", camerapos);
	
	objectShader->setMat4("model", model);
	objectShader->setMat4("view", m_View);
	objectShader->setMat4("projection", m_Proj);

	objectShader->setVec4("u_Color", color);

	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, text);
	glDrawElements(GL_TRIANGLES, indexes, GL_UNSIGNED_INT, 0);
}
