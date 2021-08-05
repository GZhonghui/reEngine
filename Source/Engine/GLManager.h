#pragma once

#include"ToolAIO.h"
#include"MathAIO.h"

class GlManager
{
public:
    GlManager() = default;
    ~GlManager() = default;

public:
    bool Init();
    bool Destroy();
};