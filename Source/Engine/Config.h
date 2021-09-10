#pragma once

#define BUILD_GAME_MODE

//Switch to Build Mode
#ifdef BUILD_GAME_MODE
const bool G_BUILD_GAME_MODE = true;
#else
const bool G_BUILD_GAME_MODE = false;
#endif

//Enable Output
const bool G_ENABLE_OUTPUT = true;

//Min Window Size
const unsigned int G_MIN_WINDOW_WIDTH = 1200;
const unsigned int G_MIN_WINDOW_HEIGHT = 768;