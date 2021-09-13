#define STB_IMAGE_IMPLEMENTATION

#include"Loader.h"

// Useless
unsigned char* Loader::ImageLoader::LoadImage(const char* Path, int& Width, int& Height)
{
    int Channels;
    auto loadRes = stbi_load(Path, &Width, &Height, &Channels, 3);

    return loadRes;
}