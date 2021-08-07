#pragma once

#include"ToolAIO.h"
#include"MathAIO.h"

class GLManager
{
public:
    GLManager() = default;
    ~GLManager() = default;

public:
    bool Init();
    bool Destroy();
};