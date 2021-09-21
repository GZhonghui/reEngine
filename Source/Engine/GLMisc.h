#pragma once

#include"ToolAIO.h"
#include"MathAIO.h"

#include"Loader.h"
#include"Shader.h"

namespace GLMisc
{
    inline unsigned int CompileShader(const char* SourceCode, sType Stage)
    {
        unsigned int ShaderID = 0;

        switch (Stage)
        {
        case sType::VERT:
        {
            ShaderID = glCreateShader(GL_VERTEX_SHADER);
        }break;

        case sType::FRAG:
        {
            ShaderID = glCreateShader(GL_FRAGMENT_SHADER);
        }break;

        default: return 0;
        }

        glShaderSource(ShaderID, 1, &SourceCode, nullptr);
        glCompileShader(ShaderID);

        int  compileSuccessfully;
        char compileInfo[512];

        glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &compileSuccessfully);
        if (!compileSuccessfully)
        {
            glGetShaderInfoLog(ShaderID, 512, NULL, compileInfo);
            Out::Log(pType::WARNING, "Shader Compile Failed : %s", compileInfo);
        }

        return ShaderID;
    }

    inline unsigned int GenDefaultTextureWithImageFile(const char* texturePath)
    {
        unsigned int textureID = 0;

        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int textureWidth, textureHeight, textureChannels;
        unsigned char* TextureData = stbi_load(texturePath, &textureWidth, &textureHeight, &textureChannels, 3);

        if (!TextureData)
        {
            Out::Log(pType::ERROR, "Generate Texture with Image Failed : %s", texturePath);
            glDeleteTextures(1, &textureID);

            return 0;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureData);
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);

        stbi_image_free(TextureData);

        return textureID;
    }
}