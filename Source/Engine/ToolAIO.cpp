#include"ToolAIO.h"

std::vector<std::string> allLogs;
std::string thisLog;

char printBuffer[128];

void Out::printTime()
{
    time_t nowTime = time(nullptr);
    tm* ltm = localtime(&nowTime);

    // printf("[%02d:%02d:%02d]", ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
    
    snprintf(printBuffer, sizeof(printBuffer), "[%02d:%02d:%02d]", ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
    thisLog += printBuffer;
}

void Out::Log(pType Type, const char* Format, ...)
{
#ifndef DEBUG_MESSAGE
    return;
#endif

    if (!G_ENABLE_OUTPUT) return;

    thisLog.clear();

    switch (Type)
    {
    case pType::MESSAGE:
        // printf("[MESSAGE]");
        thisLog += "[MESSAGE]";
        break;
    case pType::WARNING:
        // printf("[WARNING]");
        thisLog += "[WARNING]";
        break;
    case pType::ERROR:
        // printf("[ ERROR ]");
        thisLog += "[ ERROR ]";
        break;
    }
    // printf(" ");
    thisLog += " ";
    
    printTime();
    
    // printf(" >>");
    thisLog += " >>";

    va_list args;

    va_start(args, Format);
    // vprintf(Format, args);
    vsprintf(printBuffer, Format, args);
    thisLog += printBuffer;
    va_end(args);

    // puts("");

    // Max Size = 32
    if (allLogs.size() >= 32)
    {
        allLogs.erase(allLogs.begin());
    }

    allLogs.push_back(thisLog);
}