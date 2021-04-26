#include "Core.h"


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>



Core::Core()
{
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return ;
	}
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	
	
	window = glfwCreateWindow(width, height, name, 0, 0);

	

	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return;
	}
	
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwMakeContextCurrent(window);
	
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return ;
	}

	handler = new EventHandler(window);


}

Core::~Core()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
}

void Core::Loop()
{
	

	while (!end) {
		double currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		nbFrames++;
		if (currentTime - lastTime >= 1.0f / 120.0f) {
			Update();
			Draw();
			nbFrames = 0;
			lastTime = currentTime;
		}
	}
}




void Core::Init()
{




	const char* glsl_version = "#version 130";
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	text = TextureFromFile("back.jpg", "textures",true);


	ourShader = new Shader("ourShader");
	ourShader->attachShader("4.2.texture.vert")->attachShader("4.2.texture.frag")->linkPrograms();

	cube = new Shader("Cube");
	cube->attachShader("6.light_cube.vert")->attachShader("6.light_cube.frag")->linkPrograms();
	


	camera = new Camera(glm::vec3(0, 2.0f, 10.0f));
	m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	
	glEnable(GL_DEPTH_TEST);

	skyboxShader= new Shader("SkyBoxShader");
	skyboxShader->attachShader("6.2.skybox.vert")->attachShader("6.2.skybox.frag")->linkPrograms();
	skyboxShader->use();
	glUniform1i(glGetUniformLocation(skyboxShader->getID(), "skybox"), 0);
	glUniform1i(glGetUniformLocation(skyboxShader->getID(), "nightbox"), 1);

	cout << "Shader loaded!\n";
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};


	float skyboxVertices[] = {
		// positions          
		-100.0f,  100.0f, -100.0f,
		-100.0f, -100.0f, -100.0f,
		 100.0f, -100.0f, -100.0f,
		 100.0f, -100.0f, -100.0f,
		 100.0f,  100.0f, -100.0f,
		-100.0f,  100.0f, -100.0f,

		-100.0f, -100.0f,  100.0f,
		-100.0f, -100.0f, -100.0f,
		-100.0f,  100.0f, -100.0f,
		-100.0f,  100.0f, -100.0f,
		-100.0f,  100.0f,  100.0f,
		-100.0f, -100.0f,  100.0f,

		 100.0f, -100.0f, -100.0f,
		 100.0f, -100.0f,  100.0f,
		 100.0f,  100.0f,  100.0f,
		 100.0f,  100.0f,  100.0f,
		 100.0f,  100.0f, -100.0f,
		 100.0f, -100.0f, -100.0f,

		-100.0f, -100.0f,  100.0f,
		-100.0f,  100.0f,  100.0f,
		 100.0f,  100.0f,  100.0f,
		 100.0f,  100.0f,  100.0f,
		 100.0f, -100.0f,  100.0f,
		-100.0f, -100.0f,  100.0f,

		-100.0f,  100.0f, -100.0f,
		 100.0f,  100.0f, -100.0f,
		 100.0f,  100.0f,  100.0f,
		 100.0f,  100.0f,  100.0f,
		-100.0f,  100.0f,  100.0f,
		-100.0f,  100.0f, -100.0f,

		-100.0f, -100.0f, -100.0f,
		-100.0f, -100.0f,  100.0f,
		 100.0f, -100.0f, -100.0f,
		 100.0f, -100.0f, -100.0f,
		-100.0f, -100.0f,  100.0f,
		 100.0f, -100.0f,  100.0f
	};

	unsigned int VBO;
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	

	vector<std::string> faces
	{
		"textures/skybox/right.jpg",
		"textures/skybox/left.jpg",
		"textures/skybox/top.jpg",
		"textures/skybox/bottom.jpg",
		"textures/skybox/front.jpg",
		"textures/skybox/back.jpg",
	};
	cubemapTexture = loadCubemap(faces);

	vector<std::string> nightfaces
	{
		"textures/nightbox/right.png",
		"textures/nightbox/left.png",
		"textures/nightbox/top.png",
		"textures/nightbox/bottom.png",
		"textures/nightbox/front.png",
		"textures/nightbox/back.png",
	};

	nightboxTexture = loadCubemap(nightfaces);


	m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));



	objects.push_back(Object("models/cat/cat.obj", ourShader,glm::vec3(0.01f,0.01f,0.01f),0.0f, glm::vec3(0,1, 0)));
	objects.push_back(Object("models/car/car.obj", ourShader, glm::vec3(2.0f, 2.0f, 2.0f),-90.0f,glm::vec3(1,0,0)));
	objects.push_back(Object("models/deer/deer.obj", ourShader, glm::vec3(0.01f, 0.01f, 0.01f), 0.0f, glm::vec3(0, 1, 0)));
	objects.push_back(Object("models/horse/horse.obj", ourShader, glm::vec3(0.1f, 0.1f, 0.1f), -90.0f, glm::vec3(1, 0, 0)));
	objects.push_back(Object("models/single_cat/cat.obj", ourShader,glm::vec3(0.01f,0.01f,0.01f), 0.0f, glm::vec3(0, 1, 0)));
	objects.push_back(Object("models/geometry/monkey.obj", ourShader, glm::vec3(2.0f, 2.0f, 2.0f), 0.0f, glm::vec3(0, 1, 0)));




}

void Core::Update()
{



	float velocity =50.0f * deltaTime;
	if (handler->KeyPressed(GLFW_KEY_A))
		camera->Move(Camera_Movement::LEFT, velocity);

	if (handler->KeyPressed(GLFW_KEY_D))
		camera->Move(Camera_Movement::RIGHT, velocity);

	if (handler->KeyPressed(GLFW_KEY_W))
		camera->Move(Camera_Movement::FORWARD, velocity);

	if (handler->KeyPressed(GLFW_KEY_S))
		camera->Move(Camera_Movement::BACKWARD, velocity);

	if (handler->KeyPressed(GLFW_KEY_SPACE))
		camera->Move(Camera_Movement::UP, velocity);

	if (handler->KeyPressed(GLFW_KEY_LEFT_SHIFT))
		camera->Move(Camera_Movement::DOWN, velocity);

	if (locked) {
		handler->MouseHandler();
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		cout << handler->getDX() << " " << handler->gettDY() << endl;
		camera->ProcessMouseMovement(handler->getDX() * 3000 * deltaTime, handler->gettDY() * 4500 * deltaTime);

		glfwSetCursorPos(window, width / 2, height / 2);
	}
	else glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	m_Proj = glm::perspective(glm::radians(camera->Zoom), (float)width / (float)height, 0.1f, 1000.0f);
	m_View =camera->GetViewMatrix();

	if(handler->KeyPressed(GLFW_KEY_ESCAPE) ||
	glfwWindowShouldClose(window)) end = true;
	if (handler->KeyPressed(GLFW_KEY_X)) locked = !locked;
}

void Core::Draw()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glm::mat4 model;
	glm::mat4 view = glm::mat4(glm::mat3(camera->GetViewMatrix()));
	
	ourShader->use();

	for (int i = 0; i < objects.size(); i++) {
		model = glm::translate(glm::mat4(1.0f), glm::vec3(10*i, 0, 0));
		objects[i].Draw(model, m_Proj, m_View, camera->Position, glm::vec4(1.0f, 0.3f, 0.25f, 0.1f),text);
	}
	

	cube->use();
	cube->setMat4("projection", m_Proj);
	cube->setMat4("view",m_View);
	glBindVertexArray(lightCubeVAO);
	for (int i = 0; i < objects.size(); i++) {
	model = glm::mat4(1.0f);
	model = glm::translate(model, objects[i].getLightPos());
	model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
	cube->setMat4("model", model);
	}
	glDrawArrays(GL_TRIANGLES, 0, 36);


	rot += 0.01;
	time += deltaTime * delta;
	if (time > 24000)
		delta = -500;
	if (time < 0)
		delta = 500;
	// draw skybox as last
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	skyboxShader->use();
	m_View = glm::mat4(glm::mat3(camera->GetViewMatrix())); // remove translation from the view matrix
	m_View = glm::rotate(m_View, glm::radians(rot), glm::vec3(0.0, 1.0, 0.0));
	skyboxShader->setVec3("fogColour", glm::vec3(0.5444, 0.62, 0.69));
	skyboxShader->setMat4("view", m_View);
	skyboxShader->setFloat("blendFactor", (time/24000));
	skyboxShader->setMat4("projection", m_Proj);
	// skybox 
	
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, nightboxTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);


	glDepthFunc(GL_LESS); // set depth function back to default

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("Test");
	for (int i = 0; i < objects.size(); i++) {
		ImGui::TextColored(ImVec4(1, 1, 1, 1), "LightPos");
		objects[i].GUI();
	}
	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwSwapBuffers(window);
	glfwPollEvents();

}

unsigned int Core::loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrComponents;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

unsigned int Core::TextureFromFile(const char* path, const string& directory, bool gamma)
{
	string filename = string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);


	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}