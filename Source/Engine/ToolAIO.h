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
#include<imgui_internal.h>
#include<imgui_impl_glfw.h>
#include<imgui_impl_vulkan.h>
#include<imgui_impl_opengl3.h>

#include<ImGuiFileDialog.h>

#include<glad/glad.h>

#define GLFW_INCLUDE_VULKAN
#include<GLFW/glfw3.h>

#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

#include<STB/stb_image.h>
#include<STB/stb_image_write.h>

#ifdef DEBUG_MESSAGE
#undef DEBUG_MESSAGE
#endif

#define DEBUG_MESSAGE

const char True[]  = "True";
const char False[] = "False";

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

#ifdef TOOL_ALL_IN_ONE_OUT_IMPLEMENTATION
std::vector<std::string> allLogs;
std::string thisLog;

char printBuffer[512];

void Out::printTime()
{
    time_t nowTime = time(nullptr);
    tm* ltm = localtime(&nowTime);

    if (G_ENABLE_CON_OUTPUT)
    {
        printf("[%02d:%02d:%02d]", ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
    }
    if (G_ENABLE_WIN_OUTPUT)
    {
        snprintf(printBuffer, sizeof(printBuffer), "[%02d:%02d:%02d]",
            ltm->tm_hour, ltm->tm_min, ltm->tm_sec);

        thisLog += printBuffer;
    }
}

void Out::Log(pType Type, const char* Format, ...)
{
#ifndef DEBUG_MESSAGE
    return;
#endif

    thisLog.clear();

    switch (Type)
    {
    case pType::MESSAGE:
        if (G_ENABLE_CON_OUTPUT) printf("[MESSAGE]");
        if (G_ENABLE_WIN_OUTPUT) thisLog += "[MESSAGE]";
        break;
    case pType::WARNING:
        if (G_ENABLE_CON_OUTPUT) printf("[WARNING]");
        if (G_ENABLE_WIN_OUTPUT) thisLog += "[WARNING]";
        break;
    case pType::ERROR:
        if (G_ENABLE_CON_OUTPUT) printf("[ ERROR ]");
        if (G_ENABLE_WIN_OUTPUT) thisLog += "[ ERROR ]";
        break;
    }
    if (G_ENABLE_CON_OUTPUT) printf(" ");
    if (G_ENABLE_WIN_OUTPUT) thisLog += " ";

    printTime();

    if (G_ENABLE_CON_OUTPUT) printf(" >>");
    if (G_ENABLE_WIN_OUTPUT) thisLog += " >>";

    va_list args;

    va_start(args, Format);
    if (G_ENABLE_CON_OUTPUT)
    {
        vprintf(Format, args);
    }
    if (G_ENABLE_WIN_OUTPUT)
    {
        vsnprintf(printBuffer, sizeof(printBuffer), Format, args);
        thisLog += printBuffer;
    }
    va_end(args);

    if (G_ENABLE_CON_OUTPUT) puts("");

    if (G_ENABLE_WIN_OUTPUT)
    {
        // Max Size = 32
        if (allLogs.size() >= 32)
        {
            allLogs.erase(allLogs.begin());
        }

        allLogs.push_back(thisLog);
    }
}
#else
// FROM ToolAIO
extern std::vector<std::string> allLogs;
// FROM ToolAIO
#endif// TOOL_ALL_IN_ONE_OUT_IMPLEMENTATION

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

inline bool endsWith(std::string const& Value, std::string const& Ending)
{
    if (Ending.size() > Value.size()) return false;
    return std::equal(Ending.rbegin(), Ending.rend(), Value.rbegin());
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

inline int StrToInt(const std::string& inStr)
{
    int Res = -1;

    try
    {
        sscanf(inStr.c_str(), "%d", &Res);
    }
    catch (...) {}

    return Res;
}

inline std::string IntToStr(int inInt)
{
    char printBuffer[64];
    snprintf(printBuffer, sizeof(printBuffer), "%d", inInt);

    return std::string(printBuffer);
}

inline bool Inside(int x, int Left, int Right)
{
    return Left <= x && x <= Right;
}

inline void uiBeginDisable()
{
    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
}

inline void uiEndDisable()
{
    ImGui::PopItemFlag();
    ImGui::PopStyleVar();
}