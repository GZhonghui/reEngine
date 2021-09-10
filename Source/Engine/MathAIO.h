#pragma once

#include<cmath>
#include<cstdint>

#include<Eigen/Eigen>

#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<GLM/gtc/matrix_transform.hpp>

const double pi = acos(-1.0);

const double eps = 1e-6;

typedef Eigen::Vector3d Direction;
typedef Eigen::Vector3d Point;
typedef Eigen::Vector3d Color;

const Color colorGray(105 / 255.999, 105 / 255.999, 105 / 255.999);
const Color colorGreen(32 / 255.999, 127 / 255.999, 76 / 255.999);

class Transform
{
public:
    Transform() = default;
    ~Transform() = default;

public:
    Eigen::Vector3d Location;
    Eigen::Vector3d Rotation;
    Eigen::Vector3d Scale;
};

inline void Convert(const Eigen::Vector3d& From, glm::vec3& To)
{
    To[0] = From.x();
    To[1] = From.y();
    To[2] = From.z();
}

inline void Convert(const glm::vec3& From, Eigen::Vector3d& To)
{
    To.x() = From[0];
    To.y() = From[1];
    To.z() = From[2];
}

inline glm::vec3 Convert(const Eigen::Vector3d& From)
{
    return glm::vec3(From.x(), From.y(), From.z());
}

inline Eigen::Vector3d Convert(const glm::vec3& From)
{
    return Eigen::Vector3d(From[0], From[1], From[2]);
}

inline bool Near(double x, double y, double localEPS = eps)
{
    return std::abs(x - y) < localEPS;
}