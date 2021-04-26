#include "EventHandler.h"

EventHandler::~EventHandler()
{
}

bool EventHandler::KeyPressed(int keycode)
{
    return glfwGetKey(window, keycode) == GLFW_PRESS;
}

bool EventHandler::KeyReleased(int keycode)
{
    return glfwGetKey(window, keycode) == GLFW_RELEASE;
}

bool EventHandler::KeyRepeat(int keycode)
{
    return glfwGetKey(window, keycode) == GLFW_REPEAT;
}



void EventHandler::MouseHandler()
{
    glfwGetCursorPos(window, &mouseX, &mouseY);

}
