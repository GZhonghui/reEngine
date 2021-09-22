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
        const glm::mat4& Model, const glm::mat4& View, const glm::mat4& Project
    ) : m_Model(Model), m_View(View), m_Project(Project) {}

public:
    virtual ~UniformMVP() = default;
};

class Camera
{
public:
    Point m_Position;
    glm::quat m_Orientation;

    double m_rAngle;
    double m_uAngle;

    double m_FOV;

public:
    Camera() = default;

public:
    virtual ~Camera() = default;

private:
    void updateOrientation()
    {
        m_FOV = 60;

        glm::quat aroundY = glm::angleAxis(glm::radians((float)-m_rAngle), glm::vec3(0, 1, 0));

        glm::quat aroundX = glm::angleAxis(glm::radians((float)+m_uAngle), glm::vec3(1, 0, 0));

        m_Orientation = aroundY * aroundX;
    }

public:
    void Init()
    {
        m_rAngle = 180;
        m_uAngle = 0;
        updateOrientation();

        m_Position = Point(0, 5, -5);
    }

public:
    void setPosition(const Point& newPosition)
    {
        m_Position = newPosition;
    }

    Point getPosition() const noexcept
    {
        return m_Position;
    }

    Direction getForward() const noexcept
    {
        glm::quat F = m_Orientation * glm::quat(0, 0, 0, -1) * glm::conjugate(m_Orientation);
        return Direction(F.x, F.y, F.z);
    }

    Direction getRight() const noexcept
    {
        auto R = getForward().cross(cameraUP).normalized();
        if (m_uAngle < 90.0 && m_uAngle > -90.0) return R; return -R;
    }

    void setFOV(double FOV)
    {
        m_FOV = FOV;
    }

    double getFOV() const noexcept
    {
        return m_FOV;
    }

public:
    void Move(const Direction& moveOffset)
    {
        m_Position += moveOffset;
    }

    void Cursor(double x, double y, bool Ignore)
    {
        const double rotateSpeed = 0.1;

        static double lastX = x;
        static double lastY = y;

        double offsetX = (x - lastX) * rotateSpeed;
        double offsetY = (lastY - y) * rotateSpeed;

        lastX = x;
        lastY = y;

        if (Ignore) return;

        if (m_uAngle < 90.0 && m_uAngle > -90.0)
            m_rAngle += offsetX;
        else
            m_rAngle -= offsetX;

        m_uAngle += offsetY;

        while (m_uAngle > +180.0) m_uAngle -= 360.0;
        while (m_uAngle < -180.0) m_uAngle += 360.0;

        updateOrientation();
    }

public:
    bool updateView(glm::mat4& View)
    {
        glm::mat4 Rot = glm::mat4_cast(glm::conjugate(m_Orientation));
        glm::mat4 Translation = glm::translate(glm::mat4(1.0), -Convert(m_Position));

        View = Rot * Translation;

        return true;
    }

public:
    friend class Event;
};