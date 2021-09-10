#pragma once

#include"MathAIO.h"
#include"ToolAIO.h"

class ModelLoader
{
public:
    ModelLoader() = default;
    virtual ~ModelLoader() = default;
};

class ImageLoader
{
public:
    ImageLoader() = default;
    virtual ~ImageLoader() = default;

public:
    static unsigned char* LoadImage(const char* Path, int& Width, int& Height);
};