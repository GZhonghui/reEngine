// Define in One Source File
// STB_IMAGE
#define STB_IMAGE_IMPLEMENTATION
// TOOL_AIO
#define TOOL_ALL_IN_ONE_OUT_IMPLEMENTATION

#include"Init.h"
#include"EngineCore.h"

// Only On Windows
#include<Windows.h>

int main() { ShowWindow(GetConsoleWindow(), SW_HIDE); return RUN_ENGINE; }