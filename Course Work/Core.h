#pragma once



#define GLEW_STATIC
#include <GL/glew.h>



#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define GLFW_EXPOSE_NATIVE_WIN32


#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include<iostream>
#include <vector>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


using namespace std;
#include "EventHandler.h"
#include "OBJ_Loader.h"
#include "Shader.h"
#include "Camera.h"
#include "Object.h"

class Core
{
public:
	Core();
	~Core();
	void Loop();
	void Init();
	void Update();
	void Draw();
	unsigned int loadCubemap(vector<std::string> faces);
	unsigned int TextureFromFile(const char* path, const string& directory, bool gamma);

private:
	glm::mat4 m_Proj, m_View;
	Camera* camera;
	
	Shader * ourShader;
	 Shader * cube;
	Shader * skyboxShader;
	unsigned int cubemapTexture;
	unsigned int nightboxTexture;
	unsigned int skyboxVAO, skyboxVBO;
	unsigned int text;
	unsigned int lightCubeVAO;

	bool locked = false;
	vector <Object> objects;
	float rot = 0.0f;
	GLFWwindow* window;
	EventHandler* handler;
	float deltaTime;
	float time=100;
	int delta = 500;
	float width =1920, height = 1080;
	const char* name = "WindowName";
	bool end = false;
	double lastTime=0;
	int nbFrames;
	
};

