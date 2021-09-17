#pragma once

#include"ToolAIO.h"
#include"MathAIO.h"

enum class aType
{
    OBJ, TEXTURE
};

namespace AssetManager
{
    inline bool addAsset(const std::string& filePathName, const std::string& fileName)
    {
        Out::Log(pType::MESSAGE, "Loading %s", filePathName.c_str());

        try
        {
            std::filesystem::path targetPath = G_IMPORT_PATH + fileName;
            if (std::filesystem::exists(targetPath))
            {
                std::filesystem::remove(targetPath);
            }
            std::filesystem::copy_file(filePathName, targetPath);
        }
        catch (...)
        {
            Out::Log(pType::ERROR, "Import Failed");
            return false;
        }

        Out::Log(pType::MESSAGE, "Import Done");
        return true;
    }

    inline bool deleteAsset(const std::string& fileName)
    {
        Out::Log(pType::MESSAGE, "Deleting %s", fileName.c_str());

        try
        {
            std::filesystem::path targetPath = G_IMPORT_PATH + fileName;
            if (std::filesystem::exists(targetPath))
            {
                std::filesystem::remove(targetPath);
            }
            else
            {
                throw 0;
            }
        }
        catch (...)
        {
            Out::Log(pType::ERROR, "Can't Delete %s", fileName.c_str());
            return false;
        }

        Out::Log(pType::MESSAGE, "Delete Done");
        return true;
    }

    inline int getAssetList(std::vector<std::string>& assetList, std::vector<aType>& assetTypeList)
    {
        assetList.clear();
        assetTypeList.clear();

        for (const auto& singleFile : std::filesystem::directory_iterator(G_IMPORT_PATH))
        {
            std::string fileName = singleFile.path().filename().u8string();
            std::string upperName = boost::to_upper_copy<std::string>(fileName);

            bool rightType = true;
            if (endsWith(upperName, "OBJ"))
            {
                assetTypeList.push_back(aType::OBJ);
            }
            else if (endsWith(upperName, "PNG"))
            {
                assetTypeList.push_back(aType::TEXTURE);
            }
            else rightType = false;

            if (rightType)
            {
                assetList.push_back(fileName);
            }
        }

        return assetList.size();
    }
};