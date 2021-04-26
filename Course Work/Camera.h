#pragma once
#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum class Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SENSITIVITY = 0.01f;
const float ZOOM = 45.0f;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MouseSensitivity;
    float Zoom;

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }
    float Random2D(glm::vec2 st)
    {
        return glm::fract(glm::sin(glm::dot(st, glm::vec2(12.9898f, 78.233f))) * 43758.5453123f);
    }

    float InterpolatedNoise(int ind, float x, float y) {
        float integer_X = floor(x);
        float fractional_X = glm::fract(x);
        float integer_Y = floor(y);
        float fractional_Y = glm::fract(y);
        glm::vec2 randomInput = glm::vec2(integer_X, integer_Y);
        float a = Random2D(randomInput);
        float b = Random2D(randomInput + glm::vec2(1.0, 0.0));
        float c = Random2D(randomInput + glm::vec2(0.0, 1.0));
        float d = Random2D(randomInput + glm::vec2(1.0, 1.0));
       

        glm::vec2 w = glm::vec2(fractional_X, fractional_Y);
        w = w * w * w * (10.0f + w * (-15.0f + 6.0f * w));

        
        float k0 = a,
            k1 = b - a,
            k2 = c - a,
            k3 = d - c - b + a;

        return k0 + k1 * w.x + k2 * w.y + k3 * w.x * w.y;
    }

    float perlin(glm::vec2 st, int octaves, float freq, float gDispFactor) {

       
        glm::mat2 m;
        m[0][0] = 0.8;
        m[0][1] = -0.6;
        m[1][0] = 0.6;
        m[1][1] = 0.8;
      
        float persistence = 0.5;
        float total = 0.0,
            frequency = 0.005 * freq,
            amplitude = gDispFactor;
        for (int i = 0; i < octaves; ++i) {
            frequency *= 2.0;
            amplitude *= persistence;

            

            glm::vec2 v = frequency * st;

            total += InterpolatedNoise(0, v.x, v.y) * amplitude;
        }
        return total * total * total;
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void Move(Camera_Movement direction, float velocity)
    {
        switch (direction)
        {
        case Camera_Movement::FORWARD:
            Position += Front * velocity;
            break;
        case Camera_Movement::BACKWARD:
            Position -= Front * velocity;
            break;
        case Camera_Movement::LEFT:
            Position -= Right * velocity;
            break;
        case Camera_Movement::RIGHT:
            Position += Right * velocity;
            break;
        case Camera_Movement::UP:
            Position += Up * velocity;
            break;
        case Camera_Movement::DOWN:
            Position -= Up * velocity;
            break;
        default:
            break;
        }   
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up = glm::normalize(glm::cross(Right, Front));
    }
};