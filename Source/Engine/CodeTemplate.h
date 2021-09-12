#pragma once

#include"ToolAIO.h"
#include"MathAIO.h"

const std::string projectFilePath("Game.rpj");

class WorldSetting
{
public:
    WorldSetting()
    {
        m_LightDir = Direction(1, -1, 1);
        m_LightColor = Color(1, 1, 1);
        m_LightPower = 2;
    }
    virtual ~WorldSetting() = default;

public:
    Direction m_LightDir;
    Color     m_LightColor;
    double    m_LightPower;
};

class ClassItem
{
public:
    ClassItem() = default;
    virtual ~ClassItem() = default;

public:
    std::string m_Name;

    bool Render;
    std::string m_ModelFile;
    std::string m_DiffuseTextureFile;
    Color       m_DiffuseColor;
};

class ActorItem
{
public:
    ActorItem() = default;
    virtual ~ActorItem() = default;

public:
    std::string m_Name;
    std::string m_ClassName;
    std::unordered_set<std::string> m_Tags;

    Eigen::Vector3d m_Location;
    Eigen::Vector3d m_Rotation;
    Eigen::Vector3d m_Scale;
};

inline void addClassToProject(const char* className)
{
    std::string sourceFileName(className);
    sourceFileName = "impl" + sourceFileName + ".cpp";

    std::string headerFileName(className);
    headerFileName = "impl" + headerFileName + ".h";

    bool createFailed = false;

    FILE* headerFile = fopen(headerFileName.c_str(), "wt");
    if (headerFile)
    {
        fprintf(headerFile, "#include \"Actor.h\"\n");
        fprintf(headerFile, "#include \"EngineCore.h\"\n");
        fprintf(headerFile, "\n");
        fprintf(headerFile, "class %s : public Actor\n", className);
        fprintf(headerFile, "{\n");
        fprintf(headerFile, "public:\n");
        fprintf(headerFile, "    %s(const std::string& Name) : Actor(Name) { }\n",className);
        fprintf(headerFile, "    virtual ~%s() = default;\n", className);
        fprintf(headerFile, "public:\n");
        fprintf(headerFile, "    virtual void Init();\n");
        fprintf(headerFile, "    virtual void Update(float Delta);\n");
        fprintf(headerFile, "    virtual void Destroy();\n");
        fprintf(headerFile, "public:\n");
        fprintf(headerFile, "    virtual const char* getType() const noexcept\n");
        fprintf(headerFile, "    {\n");
        fprintf(headerFile, "        return \"%s\";\n", className);
        fprintf(headerFile, "    }\n");
        fprintf(headerFile, "};\n");

        fclose(headerFile);
    }
    else createFailed = true;

    FILE* sourceFile = fopen(sourceFileName.c_str(), "wt");
    if (sourceFile)
    {
        fprintf(sourceFile, "#include \"%s\"\n", headerFileName.c_str());
        fprintf(sourceFile, "\n");
        fprintf(sourceFile, "void %s::Init()\n", className);
        fprintf(sourceFile, "{\n");
        fprintf(sourceFile, "\n");
        fprintf(sourceFile, "}\n");
        fprintf(sourceFile, "\n");
        fprintf(sourceFile, "void %s::Update(float Delta)\n", className);
        fprintf(sourceFile, "{\n");
        fprintf(sourceFile, "\n");
        fprintf(sourceFile, "}\n");
        fprintf(sourceFile, "\n");
        fprintf(sourceFile, "void %s::Destroy()\n", className);
        fprintf(sourceFile, "{\n");
        fprintf(sourceFile, "\n");
        fprintf(sourceFile, "}\n");

        fclose(sourceFile);
    }
    else createFailed = true;

    if (createFailed)
    {
        Out::Log(pType::ERROR, "Create %s Failed", className);
        return;
    }

    using namespace boost::property_tree;

    ptree ptProject;
    read_xml("Engine.vcxproj", ptProject, xml_parser::trim_whitespace);

    ptree& sourceItem = ptProject.add("Project.ItemGroup.ClCompile", "");
    sourceItem.put("<xmlattr>.Include", sourceFileName.c_str());
    ptree& headerItem = ptProject.add("Project.ItemGroup.ClInclude", "");
    headerItem.put("<xmlattr>.Include", headerFileName.c_str());

    const boost::property_tree::xml_writer_settings<ptree::key_type> writeSettings(' ', 2);
    write_xml("Engine.vcxproj", ptProject, std::locale(), writeSettings);
}

inline void updateInitHeader
(
    const std::vector<ClassItem>& classItems,
    const std::vector<ActorItem>& actorItems
)
{
    FILE* initHeader = fopen("Init.h", "wt");

    if (initHeader)
    {
        fprintf(initHeader, "#pragma once\n");
        fprintf(initHeader, "\n");
        fprintf(initHeader, "#include<vector>\n");
        fprintf(initHeader, "#include<memory>\n");
        fprintf(initHeader, "\n");
        fprintf(initHeader, "#include\"Actor.h\"\n");
        fprintf(initHeader, "\n");

        //For Each
        auto addInclude = [initHeader](const char* className)
        {
            fprintf(initHeader, "#include\"impl%s.h\"\n\n", className);
        };

        for (auto classIndex = classItems.begin(); classIndex != classItems.end(); ++classIndex)
        {
            addInclude(classIndex->m_Name.c_str());
        }

        fprintf(initHeader, "inline void initScene(std::vector<std::shared_ptr<Actor>>* actorsInScene)\n");
        fprintf(initHeader, "{\n");

        //For Each
        auto addActor = [initHeader](const ActorItem& actorItem)
        {
            fprintf(initHeader, "    {\n");
            fprintf(initHeader, "        auto newActor = std::make_shared<%s>(\"%s\");\n",
                actorItem.m_ClassName.c_str(), actorItem.m_Name.c_str());

            for (auto nowTag = actorItem.m_Tags.begin(); nowTag != actorItem.m_Tags.end(); ++nowTag)
            {
                fprintf(initHeader, "        newActor->insertTag(\"%s\");\n", nowTag->c_str());
            }
            
            fprintf(initHeader, "        newActor->setLocation(Eigen::Vector3d(%.6lf, %.6lf, %.6lf));\n",
                actorItem.m_Location.x(), actorItem.m_Location.y(), actorItem.m_Location.z());
            fprintf(initHeader, "        newActor->setRotation(Eigen::Vector3d(%.6lf, %.6lf, %.6lf));\n",
                actorItem.m_Rotation.x(), actorItem.m_Rotation.y(), actorItem.m_Rotation.z());
            fprintf(initHeader, "        newActor->setScale(Eigen::Vector3d(%.6lf, %.6lf, %.6lf));\n",
                actorItem.m_Scale.x(), actorItem.m_Scale.y(), actorItem.m_Scale.z());

            fprintf(initHeader, "        actorsInScene->push_back(newActor);\n");
            fprintf(initHeader, "    }\n");
        };

        for (auto actorIndex = actorItems.begin(); actorIndex != actorItems.end(); ++actorIndex)
        {
            addActor(*actorIndex);
        }

        fprintf(initHeader, "}\n");

        fclose(initHeader);
    }
}

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
        ptree& ScaleNode    = actorItem.add("Scale",    "");

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