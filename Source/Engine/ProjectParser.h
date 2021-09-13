#pragma once

#include"CodeTemplate.h"

// Project File Struction
/*

Project
    World
        LightDir   x="" y="" z=""
        LightColor x="" y="" z=""
        LightPower

    Class
        ClassItem Name="" Render="False"
        ClassItem Name="" Render="True"
            Model
            DiffuseTexture
            DiffuseColor x="" y="" z=""

    Actor
        ActorItem Name="" ClassName=""
            Tag=""
            Location x="" y="" z=""
            Rotation x="" y="" z=""
            Scale    x="" y="" z=""
        ActorItem Name="" ClassName=""
            Tag=""
            Tag=""
            Location x="" y="" z=""
            Rotation x="" y="" z=""
            Scale    x="" y="" z=""
*/
// Project File Struction

inline void readProject
(
    WorldSetting& worldSettings,
    std::vector<ClassItem>& classItems,
    std::vector<ActorItem>& actorItems
)
{
    using namespace boost::property_tree;

    if (!std::filesystem::exists(std::filesystem::path(projectFilePath)))
    {
        Out::Log(pType::WARNING, "No Project File");
        return;
    }

    classItems.clear();
    actorItems.clear();

    std::unordered_set<std::string> classNameSet;
    std::unordered_set<std::string> actorNameSet;

    ptree ptProject;
    read_xml(projectFilePath, ptProject, xml_parser::trim_whitespace);

    try
    {
        BOOST_FOREACH(const ptree::value_type & worldItem, ptProject.get_child("Project.World"))
        {
            if (worldItem.first == "LightDir")
            {
                worldSettings.m_LightDir.x() = StrToFloat(worldItem.second.get<std::string>("<xmlattr>.x"));
                worldSettings.m_LightDir.y() = StrToFloat(worldItem.second.get<std::string>("<xmlattr>.y"));
                worldSettings.m_LightDir.z() = StrToFloat(worldItem.second.get<std::string>("<xmlattr>.z"));
            }
            else if (worldItem.first == "LightColor")
            {
                worldSettings.m_LightColor.x() = StrToFloat(worldItem.second.get<std::string>("<xmlattr>.x"));
                worldSettings.m_LightColor.y() = StrToFloat(worldItem.second.get<std::string>("<xmlattr>.y"));
                worldSettings.m_LightColor.z() = StrToFloat(worldItem.second.get<std::string>("<xmlattr>.z"));
            }
            else if (worldItem.first == "LightPower")
            {
                worldSettings.m_LightPower = StrToFloat(worldItem.second.get<std::string>(""));
            }
        }
    }
    catch (...)
    {
        Out::Log(pType::ERROR, "Read World Setting Failed");
    }

    try
    {
        BOOST_FOREACH(const ptree::value_type & classItem, ptProject.get_child("Project.Class"))
        {
            if (classItem.first == "ClassItem")
            {
                std::string thisClassName = classItem.second.get<std::string>("<xmlattr>.Name");

                if (!classNameSet.count(thisClassName))
                {
                    classNameSet.insert(thisClassName);

                    ClassItem thisClass;
                    thisClass.m_Name = thisClassName;

                    if (classItem.second.get<std::string>("<xmlattr>.Render") == "True")
                    {
                        thisClass.Render = true;
                        thisClass.m_ModelFile = classItem.second.get<std::string>("Model");
                        thisClass.m_DiffuseTextureFile = classItem.second.get<std::string>("DiffuseTexture");

                        thisClass.m_DiffuseColor.x() = StrToFloat(classItem.second.get<std::string>("DiffuseColor.<xmlattr>.x"));
                        thisClass.m_DiffuseColor.y() = StrToFloat(classItem.second.get<std::string>("DiffuseColor.<xmlattr>.y"));
                        thisClass.m_DiffuseColor.z() = StrToFloat(classItem.second.get<std::string>("DiffuseColor.<xmlattr>.z"));
                    }
                    else
                    {
                        thisClass.Render = false;
                    }

                    classItems.push_back(thisClass);
                }
            }
        }
    }
    catch (...)
    {
        Out::Log(pType::ERROR, "Read Class Failed");
    }

    try
    {
        BOOST_FOREACH(const ptree::value_type & actorItem, ptProject.get_child("Project.Actor"))
        {
            if (actorItem.first == "ActorItem")
            {
                std::string thisActorName = actorItem.second.get<std::string>("<xmlattr>.Name");
                std::string thisActorClassName = actorItem.second.get<std::string>("<xmlattr>.ClassName");

                if (!actorNameSet.count(thisActorName))
                {
                    actorNameSet.insert(thisActorName);

                    ActorItem thisActor;
                    thisActor.m_Name = thisActorName;
                    thisActor.m_ClassName = thisActorClassName;

                    BOOST_FOREACH(const ptree::value_type & infoItem, actorItem.second.get_child(""))
                    {
                        if (infoItem.first == "Tag")
                        {
                            std::string nowTag = infoItem.second.get<std::string>("");
                            thisActor.m_Tags.insert(nowTag);
                        }
                        else if (infoItem.first == "Location")
                        {
                            thisActor.m_Location.x() = StrToFloat(infoItem.second.get<std::string>("<xmlattr>.x"));
                            thisActor.m_Location.y() = StrToFloat(infoItem.second.get<std::string>("<xmlattr>.y"));
                            thisActor.m_Location.z() = StrToFloat(infoItem.second.get<std::string>("<xmlattr>.z"));
                        }
                        else if (infoItem.first == "Rotation")
                        {
                            thisActor.m_Rotation.x() = StrToFloat(infoItem.second.get<std::string>("<xmlattr>.x"));
                            thisActor.m_Rotation.y() = StrToFloat(infoItem.second.get<std::string>("<xmlattr>.y"));
                            thisActor.m_Rotation.z() = StrToFloat(infoItem.second.get<std::string>("<xmlattr>.z"));
                        }
                        else if (infoItem.first == "Scale")
                        {
                            thisActor.m_Scale.x() = StrToFloat(infoItem.second.get<std::string>("<xmlattr>.x"));
                            thisActor.m_Scale.y() = StrToFloat(infoItem.second.get<std::string>("<xmlattr>.y"));
                            thisActor.m_Scale.z() = StrToFloat(infoItem.second.get<std::string>("<xmlattr>.z"));
                        }
                    }

                    actorItems.push_back(thisActor);
                }
            }
        }
    }
    catch (...)
    {
        Out::Log(pType::ERROR, "Read Actor Failed");
    }

    Out::Log(pType::MESSAGE, "Read %d Class", classItems.size());
    Out::Log(pType::MESSAGE, "Read %d Actor", actorItems.size());
    Out::Log(pType::MESSAGE, "Read Project Done");
}

inline void saveProject
(
    WorldSetting& worldSettings,
    const std::vector<ClassItem>& classItems,
    const std::vector<ActorItem>& actorItems
)
{
    using namespace boost::property_tree;

    ptree ptProject;

    ptree& rootProject = ptProject.add("Project", "");

    ptree& worldNode = rootProject.add("World", "");
    ptree& classNode = rootProject.add("Class", "");
    ptree& actorNode = rootProject.add("Actor", "");

    ptree& lightDirNode = worldNode.add("LightDir", "");
    lightDirNode.put("<xmlattr>.x", FloatToStr(worldSettings.m_LightDir.x()).c_str());
    lightDirNode.put("<xmlattr>.y", FloatToStr(worldSettings.m_LightDir.y()).c_str());
    lightDirNode.put("<xmlattr>.z", FloatToStr(worldSettings.m_LightDir.z()).c_str());

    ptree& lightColorNode = worldNode.add("LightColor", "");
    lightColorNode.put("<xmlattr>.x", FloatToStr(worldSettings.m_LightColor.x()).c_str());
    lightColorNode.put("<xmlattr>.y", FloatToStr(worldSettings.m_LightColor.y()).c_str());
    lightColorNode.put("<xmlattr>.z", FloatToStr(worldSettings.m_LightColor.z()).c_str());

    ptree& lightPowerNode = worldNode.add("LightPower", FloatToStr(worldSettings.m_LightPower).c_str());

    for (auto classIndex = classItems.begin(); classIndex != classItems.end(); ++classIndex)
    {
        ptree& classItem = classNode.add("ClassItem", "");

        classItem.put("<xmlattr>.Name", classIndex->m_Name.c_str());
        if (classIndex->Render)
        {
            classItem.put("<xmlattr>.Render", "True");

            classItem.add("Model", classIndex->m_ModelFile.c_str());
            classItem.add("DiffuseTexture", classIndex->m_DiffuseTextureFile.c_str());

            ptree& diffuseColorNode = classItem.add("DiffuseColor", "");
            diffuseColorNode.put("<xmlattr>.x", FloatToStr(classIndex->m_DiffuseColor.x()).c_str());
            diffuseColorNode.put("<xmlattr>.y", FloatToStr(classIndex->m_DiffuseColor.y()).c_str());
            diffuseColorNode.put("<xmlattr>.z", FloatToStr(classIndex->m_DiffuseColor.z()).c_str());
        }
        else
        {
            classItem.put("<xmlattr>.Render", "False");
        }
    }

    for (auto actorIndex = actorItems.begin(); actorIndex != actorItems.end(); ++actorIndex)
    {
        ptree& actorItem = actorNode.add("ActorItem", "");

        actorItem.put("<xmlattr>.Name", actorIndex->m_Name.c_str());
        actorItem.put("<xmlattr>.ClassName", actorIndex->m_ClassName.c_str());

        for (auto nowTag = actorIndex->m_Tags.begin(); nowTag != actorIndex->m_Tags.end(); ++nowTag)
        {
            actorItem.add("Tag", nowTag->c_str());
        }

        ptree& LocationNode = actorItem.add("Location", "");
        ptree& RotationNode = actorItem.add("Rotation", "");
        ptree& ScaleNode = actorItem.add("Scale", "");

        LocationNode.put("<xmlattr>.x", FloatToStr(actorIndex->m_Location.x()).c_str());
        LocationNode.put("<xmlattr>.y", FloatToStr(actorIndex->m_Location.y()).c_str());
        LocationNode.put("<xmlattr>.z", FloatToStr(actorIndex->m_Location.z()).c_str());

        RotationNode.put("<xmlattr>.x", FloatToStr(actorIndex->m_Rotation.x()).c_str());
        RotationNode.put("<xmlattr>.y", FloatToStr(actorIndex->m_Rotation.y()).c_str());
        RotationNode.put("<xmlattr>.z", FloatToStr(actorIndex->m_Rotation.z()).c_str());

        ScaleNode.put("<xmlattr>.x", FloatToStr(actorIndex->m_Scale.x()).c_str());
        ScaleNode.put("<xmlattr>.y", FloatToStr(actorIndex->m_Scale.y()).c_str());
        ScaleNode.put("<xmlattr>.z", FloatToStr(actorIndex->m_Scale.z()).c_str());
    }

    const boost::property_tree::xml_writer_settings<ptree::key_type> writeSettings(' ', 2);
    write_xml(projectFilePath, ptProject, std::locale(), writeSettings);

    Out::Log(pType::MESSAGE, "Save File Done");

    updateInitHeader(classItems, actorItems);

    Out::Log(pType::MESSAGE, "Generated Init Header");
}