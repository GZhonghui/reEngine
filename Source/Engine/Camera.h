#pragma once

#include"MathAIO.h"
#include"ToolAIO.h"

const Direction cameraUP(0, 1, 0);

class UniformMVP
{
public:
    alignas(16) glm::mat4 m_Model;
    alignas(16) glm::mat4 m_View;
    alignas(16) glm::mat4 m_Project;

public:
    UniformMVP() = default;
    UniformMVP
    (
        const glm::mat4& model, const glm::mat4& view, const glm::mat4& project
    ) : m_Model(model), m_View(view), m_Project(project) {}

public:
    ~UniformMVP() = default;
};

class Camera
{
protected:
    Point m_Position;
    Direction m_Forward;

public:
    Camera() = default;
    Camera(const Point& position, const Direction& forward) :
        m_Position(position), m_Forward(forward.normalized()) {}

public:
    ~Camera() = default;

public:
    void setPosition(const Point& newPosition)
    {
        m_Position = newPosition;
    }

    void lookAt(const Point& target)
    {
        m_Forward = (target - m_Position).normalized();
    }

public:
    void Move(const Direction& moveOffset)
    {
        m_Position += moveOffset;
    }

public:
    bool updateMVP(float r, UniformMVP& mvp)
    {
        auto model = glm::mat4(1.0f);

        auto eye = Convert(m_Position);
        auto target = Convert(m_Position + m_Forward);

        auto view = glm::lookAt(eye, target, Convert(cameraUP));

        auto project = glm::perspective(glm::radians(45.0f), r, 0.1f, 100.0f);

        mvp = UniformMVP(model, view, project);

        return true;
    }

public:
    friend class Event;
    friend class VulkanManager;
};