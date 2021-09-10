#pragma once

#include"Config.h"

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

#include<imgui.h>
#include<imgui_impl_glfw.h>
#include<imgui_impl_vulkan.h>
#include<imgui_impl_opengl3.h>

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
    static void printTime()
    {
        time_t nowTime = time(nullptr);
        tm* ltm = localtime(&nowTime);

        printf("[%02d:%02d:%02d]", ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
    }

public:
    static void Log(pType Type, const char* Format, ...)
    {
#ifndef DEBUG_MESSAGE
        return;
#endif

        if (!G_ENABLE_OUTPUT) return;

        switch (Type)
        {
        case pType::MESSAGE:
            printf("[MESSAGE]");
            break;
        case pType::WARNING:
            printf("[WARNING]");
            break;
        case pType::ERROR:
            printf("[ ERROR ]");
            break;
        }
        printf(" "); printTime(); printf(" >>");

        va_list args;

        va_start(args, Format);
        vprintf(Format, args);
        va_end(args);

        puts("");
    }
};

inline bool checkClassOrActorName(const char* inputName)
{
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