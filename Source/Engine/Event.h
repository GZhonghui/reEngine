#pragma once

#include"MathAIO.h"
#include"ToolAIO.h"

#include"Camera.h"

class Event
{
public:
    Event() = default;
    ~Event() = default;

protected:
    static Camera mainCamera;

public:
    static bool shouldQuit;
    static bool windowResized;
    static bool mouseAsCursor;

    static float cameraMoveSpeed;

protected:
    static void resetCamera()
    {
        mainCamera.setPosition(Point(0, 0, 0));
        mainCamera.lookAt(Point(0, 0, 1));
    }

public:
    static void initEventState()
    {
        resetCamera();

        shouldQuit = false;
        windowResized = false;
        mouseAsCursor = true;

        cameraMoveSpeed = 3.0f;

        Out::Log(pType::MESSAGE, "Inited Event");
    }

public:
    static Point getCameraLocation()
    {
        return mainCamera.m_Position;
    }

    static Direction getCameraDir()
    {
        return mainCamera.m_Forward;
    }

public:
    static void glfwKeyCallback
    (
        GLFWwindow* window, int key, int scancode, int action, int mods
    )
    {
        // Push Some Key
        if (key == GLFW_KEY_Q && action == GLFW_PRESS)
        {
            // shouldQuit = true;
        }
    }

    static void glfwCursorPositionCallback
    (
        GLFWwindow* window, double x, double y
    )
    {
        const double rotateSpeed = 0.1f;

        static double yaw = 90.0;
        static double pitch = 0.0;

        static double lastX = x;
        static double lastY = y;

        double offsetX = x - lastX;
        double offsetY = y - lastY;

        lastX = x;
        lastY = y;

        if (mouseAsCursor) return;

        offsetX *= rotateSpeed;
        offsetY *= rotateSpeed;

        yaw += offsetX;
        pitch += offsetY;

        pitch = std::min(std::max(-75.0, pitch), 75.0);

        Direction forward;
        forward.x() = std::cos(glm::radians(yaw) * std::cos(glm::radians(pitch)));
        forward.y() = std::sin(glm::radians(-pitch));
        forward.z() = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));

        mainCamera.m_Forward = forward.normalized();
    }

    static void glfwMouseButtonCallback
    (
        GLFWwindow* window, int button, int action, int mods
    )
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

    static void glfwWindowResizeCallback
    (
        GLFWwindow* window, int newWidth, int newHeight
    )
    {
        windowResized = true;
    }

public:
    static void Loop(GLFWwindow* window)
    {
        const float moveSpeed = cameraMoveSpeed;

        using namespace std::chrono;

        static auto lastTime = high_resolution_clock::now();

        auto nowTime = high_resolution_clock::now();
        auto deltaTime = duration<float, seconds::period>(nowTime - lastTime).count();

        lastTime = nowTime;

        if (mouseAsCursor) return;

        Direction cameraRight = mainCamera.m_Forward.cross(cameraUP);

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            mainCamera.Move(mainCamera.m_Forward * -1 * deltaTime * moveSpeed);
        }
        else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            mainCamera.Move(mainCamera.m_Forward * deltaTime * moveSpeed);
        }
        else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            mainCamera.Move(cameraRight * -1 * deltaTime * moveSpeed);
        }
        else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            mainCamera.Move(cameraRight * deltaTime * moveSpeed);
        }
    }

public:
    friend class VulkanManager;
    friend class GLManager;
};