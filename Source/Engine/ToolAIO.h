#pragma once

//Switch to Build Mode
//#define BUILD_GAME

//Construction Function
#pragma warning(disable:26495)

//enum class in Vulkan SDK
#pragma warning(disable:26812)

#include<unordered_set>
#include<unordered_map>
#include<algorithm>
#include<cstdint>
#include<cstdarg>
#include<chrono>
#include<vector>
#include<cstdio>
#include<ctime>

#include<imgui.h>
#include<imgui_impl_glfw.h>
#include<imgui_impl_vulkan.h>

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