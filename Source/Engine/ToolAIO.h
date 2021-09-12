#pragma once

#include"Config.h"

// sscanf
#pragma warning(disable:6031)

//Construction Function
#pragma warning(disable:26495)

//enum class in Vulkan SDK
#pragma warning(disable:26812)

#include<unordered_set>
#include<unordered_map>
#include<filesystem>
#include<algorithm>
#include<cstdint>
#include<cstdarg>
#include<chrono>
#include<vector>
#include<cstdio>
#include<ctime>

#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree/xml_parser.hpp>
#include<boost/foreach.hpp>
#include<boost/lexical_cast.hpp>
#include<boost/algorithm/string.hpp>

#include<imgui.h>
#include<imgui_impl_glfw.h>
#include<imgui_impl_vulkan.h>
#include<imgui_impl_opengl3.h>

#include<ImGuiFileDialog.h>

#include<glad/glad.h>

#define GLFW_INCLUDE_VULKAN
#include<GLFW/glfw3.h>

#include<assimp/scene.h>

#include<STB/stb_image.h>
#include<STB/stb_image_write.h>

#ifdef DEBUG_MESSAGE
#undef DEBUG_MESSAGE
#endif

#define DEBUG_MESSAGE

enum class pType
{
    MESSAGE, WARNING, ERROR
};

class Out
{
public:
    Out() = default;
    ~Out() = default;

private:
    static void printTime();

public:
    static void Log(pType Type, const char* Format, ...);
};

inline bool checkSimpleStr(const char* inputName)
{
    if (!(*inputName)) return false;

    auto checkIndex = inputName;

    auto isAlpha = [](char x)->bool
    {
        return ('a' <= x && x <= 'z') || ('A' <= x && x < 'Z');
    };

    auto isNumber = [](char x)->bool
    {
        return '0' <= x && x <= '9';
    };

    while (*checkIndex)
    {
        if (!isAlpha(*checkIndex) && !isNumber(*checkIndex) && (*checkIndex != '_'))
        {
            return false;
        }
        ++checkIndex;
    }

    return true;
}

inline bool endsWith(std::string const& value, std::string const& ending)
{
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

inline double StrToFloat(const std::string& inStr)
{
    double Res = -1;
    try
    {
        sscanf(inStr.c_str(), "%lf", &Res);
    }
    catch (...) {}

    return Res;
}

inline std::string FloatToStr(double inFloat)
{
    char printBuffer[64];
    snprintf(printBuffer, sizeof(printBuffer), "%.10lf", inFloat);

    return std::string(printBuffer);
}