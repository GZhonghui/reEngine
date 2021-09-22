#pragma once

#include"MathAIO.h"
#include"ToolAIO.h"

#include"Camera.h"

class Event
{
public:
    Event() = default;
    virtual ~Event() = default;

public:
    static Camera mainCamera;

    static bool shouldQuit;
    static bool windowResized;
    static bool mouseAsCursor;

    static double cameraMoveSpeed;

public:
    static void initEventState()
    {
        mainCamera.Init();

        shouldQuit    = false;
        windowResized = false;
        mouseAsCursor = true;

        cameraMoveSpeed = 3.0;

        Out::Log(pType::MESSAGE, "Inited Event");
    }

public:
    static void glfwKeyCallback
    (
        GLFWwindow* window, int key, int scancode, int action, int mods
    )
    {
        if (G_BUILD_GAME_MODE)
        {

        }
        else
        {
            if (key == GLFW_KEY_Q && action == GLFW_PRESS);
        }
    }

    static void glfwCursorPositionCallback
    (
        GLFWwindow* window, double x, double y
    )
    {
        if (G_BUILD_GAME_MODE)
        {

        }
        else
        {
            mainCamera.Cursor(x, y, mouseAsCursor);
        }
    }

    static void glfwMouseButtonCallback
    (
        GLFWwindow* window, int button, int action, int mods
    )
    {
        if (G_BUILD_GAME_MODE)
        {

        }
        else
        {
            if (button == GLFW_MOUSE_BUTTON_RIGHT)
            {
                if (action == GLFW_PRESS)
                {
                    mouseAsCursor = false;
                }
                else if (action == GLFW_RELEASE)
                {
                    mouseAsCursor = true;
                }

                if (mouseAsCursor)
                {
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                }
                else
                {
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                }
            }
        }
    }

    static void glfwWindowResizeCallback
    (
        GLFWwindow* window, int newWidth, int newHeight
    )
    {
        if (G_BUILD_GAME_MODE)
        {

        }
        else
        {
            windowResized = true;
        }
    }

public:
    static void Loop(GLFWwindow* window)
    {
        if (G_BUILD_GAME_MODE)
        {
            
        }
        else
        {
            using namespace std::chrono;

            static auto lastTime = high_resolution_clock::now();

            auto nowTime   = high_resolution_clock::now();
            auto deltaTime = duration<float, seconds::period>(nowTime - lastTime).count();

            lastTime = nowTime;

            if (mouseAsCursor) return;

            Direction cameraForward = mainCamera.getForward();
            Direction cameraRight   = mainCamera.getRight();

            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            {
                mainCamera.Move(cameraForward * deltaTime * cameraMoveSpeed * -1);
            }
            else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            {
                mainCamera.Move(cameraForward * deltaTime * cameraMoveSpeed);
            }
            else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            {
                mainCamera.Move(cameraRight * deltaTime * cameraMoveSpeed * -1);
            }
            else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            {
                mainCamera.Move(cameraRight * deltaTime * cameraMoveSpeed);
            }
        }
    }

public:
    friend class VulkanManager;
    friend class GLManager;
};