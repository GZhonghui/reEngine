#pragma once

#include"ToolAIO.h"
#include"MathAIO.h"

#include"GLRenderable.h"

class Actor
{
public:
    Actor(const std::string& Name) :m_Name(Name) {}
    virtual ~Actor() = default;
protected:
    std::string m_Name;
    std::unordered_set<std::string> m_Tags;
public:
    std::string getName() const noexcept { return m_Name; }
    bool hasTag(const std::string& checkTag) const { return m_Tags.count(checkTag); }
    void insertTag(const std::string& Tag) { m_Tags.insert(Tag); }
    void deleteTag(const std::string& Tag) { m_Tags.erase(Tag); }
public:
    virtual void Init() = 0;
    virtual void Update(float Delta) = 0;
    virtual void Destroy() = 0;
public:
    virtual const char* getType() const noexcept = 0;
};