#pragma once

#include"ToolAIO.h"
#include"MathAIO.h"

const std::string projectFilePath("Game.rpj");

class WorldSetting
{
public:
    WorldSetting()
    {
        m_LightDir   = Direction(1, -1, 1);
        m_LightColor = Color(1, 1, 1);
        m_LightPower = 2;
    }
    virtual ~WorldSetting() = default;

public:
    Direction m_LightDir;
    Color     m_LightColor;
    double    m_LightPower;

    std::string m_Skybox;
};

class ActorItem
{
public:
    ActorItem()
    {
        m_Location = Eigen::Vector3d(0, 0, 0);
        m_Rotation = Eigen::Vector3d(0, 0, 0);
        m_Scale    = Eigen::Vector3d(1, 1, 1);
    }
    virtual ~ActorItem() = default;

public:
    std::string m_Name;
    std::string m_ClassName;
    std::unordered_set<std::string> m_Tags;
    std::unordered_set<std::string> m_Components;

    Eigen::Vector3d m_Location;
    Eigen::Vector3d m_Rotation;
    Eigen::Vector3d m_Scale;
};

class ClassItem
{
public:
    ClassItem()
    {
        m_Render = false;

        m_DiffuseColor = Color(1, 1, 1);

        m_EnableDiffuseTexture  = false;
        m_EnableNormalTexture   = false;
        m_EnableSpecularTexture = false;

        m_N = 1.4;
    }
    virtual ~ClassItem() = default;

public:
    std::string m_Name;

    bool m_Render;
    std::string m_Model;

    std::string m_Shader;
    Color m_DiffuseColor;

    bool m_EnableDiffuseTexture;
    std::string m_DiffuseTexture;

    bool m_EnableNormalTexture;
    std::string m_NormalTexture;

    bool m_EnableSpecularTexture;
    std::string m_SpecularTexture;

    // Refractive index
    double m_N;
};

class ComponentItem
{
public:
    ComponentItem() { }
    virtual ~ComponentItem() = default;

public:
    std::string m_Name;
};

inline void addClassToProject(const char* className)
{
    std::string sourceFileName(className);
    sourceFileName = "implClass_" + sourceFileName + ".cpp";

    std::string headerFileName(className);
    headerFileName = "implClass_" + headerFileName + ".h";

    bool createFailed = false;

    FILE* headerFile = fopen(headerFileName.c_str(), "wt");
    if (headerFile)
    {
        fprintf(headerFile, "#include \"Actor.h\"\n");
        fprintf(headerFile, "#include \"EngineAPI.h\"\n");
        fprintf(headerFile, "\n");
        fprintf(headerFile, "namespace UserClass\n");
        fprintf(headerFile, "{\n");
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
        fprintf(headerFile, "};\n");

        fclose(headerFile);
    }
    else createFailed = true;

    FILE* sourceFile = fopen(sourceFileName.c_str(), "wt");
    if (sourceFile)
    {
        fprintf(sourceFile, "#include \"%s\"\n", headerFileName.c_str());
        fprintf(sourceFile, "\n");
        fprintf(headerFile, "namespace UserClass\n");
        fprintf(headerFile, "{\n");
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
        fprintf(headerFile, "};\n");

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

inline void addComponentToProject(const char* componentName)
{
    std::string sourceFileName(componentName);
    sourceFileName = "implComponent_" + sourceFileName + ".cpp";

    std::string headerFileName(componentName);
    headerFileName = "implComponent_" + headerFileName + ".h";

    bool createFailed = false;

    FILE* headerFile = fopen(headerFileName.c_str(), "wt");
    if (headerFile)
    {
        fprintf(headerFile, "#include \"Component.h\"\n");
        fprintf(headerFile, "#include \"EngineAPI.h\"\n");
        fprintf(headerFile, "\n");
        fprintf(headerFile, "namespace UserComponent\n");
        fprintf(headerFile, "{\n");
        fprintf(headerFile, "class %s : public Component\n", componentName);
        fprintf(headerFile, "{\n");
        fprintf(headerFile, "public:\n");
        fprintf(headerFile, "    %s(const std::string& Owner) : Component(Owner) { }\n", componentName);
        fprintf(headerFile, "    virtual ~%s() = default;\n", componentName);
        fprintf(headerFile, "public:\n");
        fprintf(headerFile, "    virtual void Init();\n");
        fprintf(headerFile, "    virtual void Update(float Delta);\n");
        fprintf(headerFile, "    virtual void Destroy();\n");
        fprintf(headerFile, "};\n");
        fprintf(headerFile, "};\n");

        fclose(headerFile);
    }
    else createFailed = true;

    FILE* sourceFile = fopen(sourceFileName.c_str(), "wt");
    if (sourceFile)
    {
        fprintf(sourceFile, "#include \"%s\"\n", headerFileName.c_str());
        fprintf(sourceFile, "\n");
        fprintf(headerFile, "namespace UserComponent\n");
        fprintf(headerFile, "{\n");
        fprintf(sourceFile, "void %s::Init()\n", componentName);
        fprintf(sourceFile, "{\n");
        fprintf(sourceFile, "\n");
        fprintf(sourceFile, "}\n");
        fprintf(sourceFile, "\n");
        fprintf(sourceFile, "void %s::Update(float Delta)\n", componentName);
        fprintf(sourceFile, "{\n");
        fprintf(sourceFile, "\n");
        fprintf(sourceFile, "}\n");
        fprintf(sourceFile, "\n");
        fprintf(sourceFile, "void %s::Destroy()\n", componentName);
        fprintf(sourceFile, "{\n");
        fprintf(sourceFile, "\n");
        fprintf(sourceFile, "}\n");
        fprintf(headerFile, "};\n");

        fclose(sourceFile);
    }
    else createFailed = true;

    if (createFailed)
    {
        Out::Log(pType::ERROR, "Create %s Failed", componentName);
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
    const std::vector<ActorItem>& actorItems,
    const std::vector<ComponentItem>& componentItems
)
{
    FILE* initHeader = fopen("Init.h", "wt");

    if (initHeader)
    {
        // Default Code
        fprintf(initHeader, "/*\n");
        fprintf(initHeader, "=== Default Code ===\n");
        fprintf(initHeader, "\n");
        fprintf(initHeader, "#pragma once\n");
        fprintf(initHeader, "\n");
        fprintf(initHeader, "#include<vector>\n");
        fprintf(initHeader, "#include<memory>\n");
        fprintf(initHeader, "\n");
        fprintf(initHeader, "#include\"Actor.h\"\n");
        fprintf(initHeader, "#include\"Component.h\"\n");
        fprintf(initHeader, "\n");
        fprintf(initHeader, "void initScene(std::vector<std::shared_ptr<Actor>>* actorsInScene)\n");
        fprintf(initHeader, "{\n");
        fprintf(initHeader, "}\n");
        fprintf(initHeader, "\n");
        fprintf(initHeader, "*/\n");
        fprintf(initHeader, "\n");
        // Default Code

        fprintf(initHeader, "#pragma once\n");
        fprintf(initHeader, "\n");
        fprintf(initHeader, "#include<vector>\n");
        fprintf(initHeader, "#include<memory>\n");
        fprintf(initHeader, "\n");
        fprintf(initHeader, "#include\"Actor.h\"\n");
        fprintf(initHeader, "\n");

        //For Each
        auto addClassInclude = [initHeader](const char* className)
        {
            fprintf(initHeader, "#include\"implClass_%s.h\"\n\n", className);
        };

        auto addComponentInclde = [initHeader](const char* componentName)
        {
            fprintf(initHeader, "#include\"implComponent_%s.h\"\n\n", componentName);
        };

        for (auto classIndex = classItems.begin(); classIndex != classItems.end(); ++classIndex)
        {
            addClassInclude(classIndex->m_Name.c_str());
        }

        for (auto componentIndex = componentItems.begin(); componentIndex != componentItems.end(); ++componentIndex)
        {
            addComponentInclde(componentIndex->m_Name.c_str());
        }

        fprintf(initHeader, "void initScene(std::vector<std::shared_ptr<Actor>>* actorsInScene)\n");
        fprintf(initHeader, "{\n");

        //For Each
        auto addActor = [initHeader](const ActorItem& actorItem)
        {
            fprintf(initHeader, "    {\n");
            fprintf(initHeader, "        auto newActor = std::make_shared<UserClass::%s>(\"%s\");\n",
                actorItem.m_ClassName.c_str(), actorItem.m_Name.c_str());

            for (auto nowTag = actorItem.m_Tags.begin(); nowTag != actorItem.m_Tags.end(); ++nowTag)
            {
                fprintf(initHeader, "        newActor->insertTag(\"%s\");\n", nowTag->c_str());
            }

            for (auto nowComponent = actorItem.m_Components.begin(); nowComponent != actorItem.m_Components.end(); ++nowComponent)
            {
                fprintf(initHeader, "        newActor->addComponent(std::make_shared<UserComponent::%s>(\"%s\"));\n",
                    nowComponent->c_str(), actorItem.m_Name.c_str());
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