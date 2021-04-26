#pragma once
#include "OBJ_Loader.h"
#include <string>
#include <vector>
#include <GL/glew.h>
#include "Shader.h"
using namespace std;

class Object
{
public:
	Object();
	Object(std::string path,Shader* shader,glm::vec3 scale,float angle,glm::vec3 rot );
	~Object();
	void GUI();
	glm::vec3 getLightPos() { return lightning; }
	void Draw(glm::mat4 model, glm::mat4 m_Proj, glm::mat4 m_View, glm::vec3 camerapos, glm::vec4 color, unsigned int text);

private:
	vector < objl::Vertex> vertex;
	std::vector<unsigned int> LoadedIndices;
	glm::vec3 lightning;
	glm::vec3 object_scale;
	glm::vec3 object_rot;
	float object_angle;
	objl::Loader Loader;
	GLuint VAO , VBO, EBO;
	unsigned int indexes;
	Shader* objectShader;

};

