#pragma once
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#define GLFW_EXPOSE_NATIVE_WIN32
class EventHandler
{
public:
	EventHandler();
	EventHandler(GLFWwindow* window) {
		this->window = window;
	};
	~EventHandler();

	bool KeyPressed(int keycode);
	bool KeyReleased(int keycode);
	bool KeyRepeat(int keycode);
	void MouseHandler();
	double getDX() {
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return x - mouseX;
	}
	double gettDY() {
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return mouseY - y;
	}
private:
	GLFWwindow* window;
	double mouseX, mouseY;
	double deltaX, deltaY;
};

