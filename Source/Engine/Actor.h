#pragma once

#include"Object.h"
#include"Component.h"

class Actor : Object
{
public:
    Actor(const std::string& Name) :m_Name(Name) {}
    virtual ~Actor() = default;
protected:
    std::vector<std::shared_ptr<Component>> m_Components;
protected:
    std::string m_Name;
    std::unordered_set<std::string> m_Tags;
    Eigen::Vector3d m_Location;
    Eigen::Vector3d m_Rotation;
    Eigen::Vector3d m_Scale;
public:
    void setLocation(const Eigen::Vector3d& newLocation) { m_Location = newLocation; }
    void setRotation(const Eigen::Vector3d& newRotation) { m_Rotation = newRotation; }
    void setScale   (const Eigen::Vector3d& newScale)    { m_Scale    = newScale;    }
    Eigen::Vector3d getLocation() const { return m_Location; }
    Eigen::Vector3d getRotation() const { return m_Rotation; }
    Eigen::Vector3d getScale()    const { return m_Scale;    }
public:
    std::string getName() const noexcept { return m_Name; }
    bool hasTag(const std::string& checkTag) const { return m_Tags.count(checkTag); }
    void insertTag(const std::string& Tag) { m_Tags.insert(Tag); }
    void deleteTag(const std::string& Tag) { m_Tags.erase(Tag);  }
public:
    virtual void Init() = 0;
    virtual void Update(float Delta) = 0;
    virtual void Destroy() = 0;
public:
    virtual const char* getType() const noexcept = 0;
};