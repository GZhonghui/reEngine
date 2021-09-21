#pragma once

#include"CodeTemplate.h"

// Project File Struction
/*

Project
    World
        LightDir   x="" y="" z=""
        LightColor x="" y="" z=""
        LightPower
        Skybox

    Actor
        ActorItem Name="" ClassName=""
            Tag=""
            Component=""
            Component=""
            Location x="" y="" z=""
            Rotation x="" y="" z=""
            Scale    x="" y="" z=""
        ActorItem Name="" ClassName=""
            Tag=""
            Tag=""
            Component=""
            Location x="" y="" z=""
            Rotation x="" y="" z=""
            Scale    x="" y="" z=""

    Class
        ClassItem Name="" Render="False"
        ClassItem Name="" Render="True"
            Model
            Shader
            DiffuseColor x="" y="" z=""
            DiffuseTexture Enable="True/False"
            NormalTexture Enable="True/False"
            SpecularTexture Enable="True/False"
            N

    Component
        ComponentItem Name=""
*/
// Project File Struction

inline void readProject
(
    WorldSetting& worldSettings,
    std::vector<ActorItem>& actorItems,
    std::vector<ClassItem>& classItems,
    std::vector<ComponentItem>& componentItems
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
    componentItems.clear();

    std::unordered_set<std::string> classNameSet;
    std::unordered_set<std::string> actorNameSet;
    std::unordered_set<std::string> componentNameSet;

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
            else if (worldItem.first == "Skybox")
            {
                worldSettings.m_Skybox = worldItem.second.get<std::string>("");
            }
        }
    }
    catch (...)
    {
        Out::Log(pType::ERROR, "Read World Setting Failed");
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
                        else if (infoItem.first == "Component")
                        {
                            std::string nowComponent = infoItem.second.get<std::string>("");
                            thisActor.m_Components.insert(nowComponent);
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

                    if (classItem.second.get<std::string>("<xmlattr>.Render") == True)
                    {
                        thisClass.m_Render = true;
                        thisClass.m_Model  = classItem.second.get<std::string>("Model");
                        thisClass.m_Shader = classItem.second.get<std::string>("Shader");

                        thisClass.m_DiffuseColor.x() = StrToFloat(classItem.second.get<std::string>("DiffuseColor.<xmlattr>.x"));
                        thisClass.m_DiffuseColor.y() = StrToFloat(classItem.second.get<std::string>("DiffuseColor.<xmlattr>.y"));
                        thisClass.m_DiffuseColor.z() = StrToFloat(classItem.second.get<std::string>("DiffuseColor.<xmlattr>.z"));

                        thisClass.m_EnableDiffuseTexture = 
                            classItem.second.get<std::string>("DiffuseTexture.<xmlattr>.Enable") == True;
                        thisClass.m_DiffuseTexture = classItem.second.get<std::string>("DiffuseTexture");

                        thisClass.m_EnableNormalTexture =
                            classItem.second.get<std::string>("NormalTexture.<xmlattr>.Enable") == True;
                        thisClass.m_NormalTexture = classItem.second.get<std::string>("NormalTexture");

                        thisClass.m_EnableSpecularTexture =
                            classItem.second.get<std::string>("SpecularTexture.<xmlattr>.Enable") == True;
                        thisClass.m_SpecularTexture = classItem.second.get<std::string>("SpecularTexture");

                        thisClass.m_N = StrToFloat(classItem.second.get<std::string>("N"));
                    }
                    else
                    {
                        thisClass.m_Render = false;
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
        BOOST_FOREACH(const ptree::value_type & componentItem, ptProject.get_child("Project.Component"))
        {
            if (componentItem.first == "ComponentItem")
            {
                std::string thisComponentName = componentItem.second.get<std::string>("<xmlattr>.Name");

                if (!componentNameSet.count(thisComponentName))
                {
                    componentNameSet.insert(thisComponentName);

                    ComponentItem thisComponent;
                    thisComponent.m_Name = thisComponentName;

                    componentItems.push_back(thisComponent);
                }
            }
        }
    }
    catch (...)
    {
        Out::Log(pType::ERROR, "Read Component Failed");
    }

    Out::Log(pType::MESSAGE, "Read %d Class", classItems.size());
    Out::Log(pType::MESSAGE, "Read %d Actor", actorItems.size());
    Out::Log(pType::MESSAGE, "Read %d Component", componentItems.size());
    Out::Log(pType::MESSAGE, "Read Project Done");
}

inline void saveProject
(
    const WorldSetting& worldSettings,
    const std::vector<ActorItem>& actorItems,
    const std::vector<ClassItem>& classItems,
    const std::vector<ComponentItem>& componentItems
)
{
    using namespace boost::property_tree;

    ptree ptProject;

    ptree& rootProject = ptProject.add("Project", "");

    ptree& worldNode = rootProject.add("World", "");
    ptree& actorNode = rootProject.add("Actor", "");
    ptree& classNode = rootProject.add("Class", "");
    ptree& componentNode = rootProject.add("Component", "");

    ptree& lightDirNode = worldNode.add("LightDir", "");
    lightDirNode.put("<xmlattr>.x", FloatToStr(worldSettings.m_LightDir.x()));
    lightDirNode.put("<xmlattr>.y", FloatToStr(worldSettings.m_LightDir.y()));
    lightDirNode.put("<xmlattr>.z", FloatToStr(worldSettings.m_LightDir.z()));

    ptree& lightColorNode = worldNode.add("LightColor", "");
    lightColorNode.put("<xmlattr>.x", FloatToStr(worldSettings.m_LightColor.x()));
    lightColorNode.put("<xmlattr>.y", FloatToStr(worldSettings.m_LightColor.y()));
    lightColorNode.put("<xmlattr>.z", FloatToStr(worldSettings.m_LightColor.z()));

    worldNode.add("LightPower", FloatToStr(worldSettings.m_LightPower));

    worldNode.add("Skybox", worldSettings.m_Skybox);

    for (auto actorIndex = actorItems.begin(); actorIndex != actorItems.end(); ++actorIndex)
    {
        ptree& actorItem = actorNode.add("ActorItem", "");

        actorItem.put("<xmlattr>.Name", actorIndex->m_Name.c_str());
        actorItem.put("<xmlattr>.ClassName", actorIndex->m_ClassName.c_str());

        for (auto nowTag = actorIndex->m_Tags.begin(); nowTag != actorIndex->m_Tags.end(); ++nowTag)
        {
            actorItem.add("Tag", nowTag->c_str());
        }

        auto pComponents = &actorIndex->m_Components;
        for (auto nowComponent = pComponents->begin(); nowComponent != pComponents->end(); ++nowComponent)
        {
            actorItem.add("Component", nowComponent->c_str());
        }

        ptree& LocationNode = actorItem.add("Location", "");
        ptree& RotationNode = actorItem.add("Rotation", "");
        ptree& ScaleNode    = actorItem.add("Scale", "");

        LocationNode.put("<xmlattr>.x", FloatToStr(actorIndex->m_Location.x()));
        LocationNode.put("<xmlattr>.y", FloatToStr(actorIndex->m_Location.y()));
        LocationNode.put("<xmlattr>.z", FloatToStr(actorIndex->m_Location.z()));

        RotationNode.put("<xmlattr>.x", FloatToStr(actorIndex->m_Rotation.x()));
        RotationNode.put("<xmlattr>.y", FloatToStr(actorIndex->m_Rotation.y()));
        RotationNode.put("<xmlattr>.z", FloatToStr(actorIndex->m_Rotation.z()));

        ScaleNode.put("<xmlattr>.x", FloatToStr(actorIndex->m_Scale.x()));
        ScaleNode.put("<xmlattr>.y", FloatToStr(actorIndex->m_Scale.y()));
        ScaleNode.put("<xmlattr>.z", FloatToStr(actorIndex->m_Scale.z()));
    }

    for (auto classIndex = classItems.begin(); classIndex != classItems.end(); ++classIndex)
    {
        ptree& classItem = classNode.add("ClassItem", "");

        classItem.put("<xmlattr>.Name", classIndex->m_Name);

        if (classIndex->m_Render)
        {
            classItem.put("<xmlattr>.Render", "True");

            classItem.add("Model", classIndex->m_Model);
            classItem.add("Shader", classIndex->m_Shader);

            ptree& diffuseColorNode = classItem.add("DiffuseColor", "");
            diffuseColorNode.put("<xmlattr>.x", FloatToStr(classIndex->m_DiffuseColor.x()));
            diffuseColorNode.put("<xmlattr>.y", FloatToStr(classIndex->m_DiffuseColor.y()));
            diffuseColorNode.put("<xmlattr>.z", FloatToStr(classIndex->m_DiffuseColor.z()));

            ptree& diffuseNode = classItem.add("DiffuseTexture", "");
            diffuseNode.put("<xmlattr>.Enable", classIndex->m_EnableDiffuseTexture ? True : False);
            if (classIndex->m_EnableDiffuseTexture) diffuseNode.put("", classIndex->m_DiffuseTexture);

            ptree& normalNode = classItem.add("NormalTexture", "");
            normalNode.put("<xmlattr>.Enable", classIndex->m_EnableNormalTexture ? True : False);
            if (classIndex->m_EnableNormalTexture) normalNode.put("", classIndex->m_NormalTexture);

            ptree& specularNode = classItem.add("SpecularTexture", "");
            specularNode.put("<xmlattr>.Enable", classIndex->m_EnableSpecularTexture ? True : False);
            if (classIndex->m_EnableSpecularTexture)specularNode.put("", classIndex->m_SpecularTexture);

            classItem.add("N", FloatToStr(classIndex->m_N));
        }
        else
        {
            classItem.put("<xmlattr>.Render", "False");
        }
    }

    for (auto componentIndex = componentItems.begin(); componentIndex != componentItems.end(); ++componentIndex)
    {
        ptree& componentItem = componentNode.add("ComponentItem","");
        componentItem.put("<xmlattr>.Name", componentIndex->m_Name);
    }

    const boost::property_tree::xml_writer_settings<ptree::key_type> writeSettings(' ', 2);
    write_xml(projectFilePath, ptProject, std::locale(), writeSettings);

    updateInitHeader(classItems, actorItems, componentItems);
    Out::Log(pType::MESSAGE, "Generated Init Header");

    Out::Log(pType::MESSAGE, "Save File Done");
}