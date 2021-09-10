#pragma once

#include"ToolAIO.h"
#include"MathAIO.h"

const std::string projectFilePath("Game.rpj");

class ClassItem
{
public:
    ClassItem() = default;
    ~ClassItem() = default;

public:
    std::string m_Name;
};

class ActorItem
{
public:
    ActorItem() = default;
    ~ActorItem() = default;

public:
    std::string m_Name;
    std::string m_ClassName;
    std::unordered_set<std::string> m_Tags;
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
        auto addActor = [initHeader](const char* actorName, const char* className, const std::unordered_set<std::string>& Tags)
        {
            fprintf(initHeader, "    {\n");
            fprintf(initHeader, "        auto newActor = std::make_shared<%s>(\"%s\");\n", className, actorName);
            for (auto nowTag = Tags.begin(); nowTag != Tags.end(); ++nowTag)
            {
                fprintf(initHeader, "        newActor->insertTag(\"%s\");\n", nowTag->c_str());
            }
            fprintf(initHeader, "        actorsInScene->push_back(newActor);\n");
            fprintf(initHeader, "    }\n");
        };

        for (auto actorIndex = actorItems.begin(); actorIndex != actorItems.end(); ++actorIndex)
        {
            addActor(actorIndex->m_Name.c_str(), actorIndex->m_ClassName.c_str(), actorIndex->m_Tags);
        }

        fprintf(initHeader, "}\n");

        fclose(initHeader);
    }
}

// Project File Struction
/*

Project
    Class
        ClassItem
            Name=""
        ClassItem
            Name=""

    Actor
        ActorItem
            Name=""
            ClassName=""
            Tag=""
            Tag=""
        ActorItem
            Name=""
            ClassName=""
            Tag=""
*/
// Project File Struction

inline void readProject
(
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

                    classItems.push_back(thisClass);
                }
            }
        }
    }
    catch (...) {}
    
    try
    {
        BOOST_FOREACH(const ptree::value_type & actorItem, ptProject.get_child("Project.Actor"))
        {
            if (actorItem.first == "ActorItem")
            {
                std::string thisActorName = actorItem.second.get<std::string>("<xmlattr>.Name");
                std::string thisActorClassName = actorItem.second.get<std::string>("ClassName");

                if (!actorNameSet.count(thisActorName))
                {
                    actorNameSet.insert(thisActorName);

                    ActorItem thisActor;
                    thisActor.m_Name = thisActorName;
                    thisActor.m_ClassName = thisActorClassName;

                    BOOST_FOREACH(const ptree::value_type & tagItem, actorItem.second.get_child(""))
                    {
                        if (tagItem.first == "Tag")
                        {
                            std::string nowTag = tagItem.second.get<std::string>("");
                            thisActor.m_Tags.insert(nowTag);
                        }
                    }

                    actorItems.push_back(thisActor);
                }
            }
        }
    }
    catch (...) {}

    Out::Log(pType::MESSAGE, "Read %d Class", classItems.size());
    Out::Log(pType::MESSAGE, "Read %d Actor", actorItems.size());
}

inline void saveProject
(
    const std::vector<ClassItem>& classItems,
    const std::vector<ActorItem>& actorItems
)
{
    using namespace boost::property_tree;

    ptree ptProject;

    for (auto classIndex = classItems.begin(); classIndex != classItems.end(); ++classIndex)
    {
        ptree& classItem = ptProject.add("Project.Class.ClassItem", "");
        classItem.put("<xmlattr>.Name", classIndex->m_Name.c_str());
    }

    for (auto actorIndex = actorItems.begin(); actorIndex != actorItems.end(); ++actorIndex)
    {
        ptree& actorItem = ptProject.add("Project.Actor.ActorItem", "");
        actorItem.put("<xmlattr>.Name", actorIndex->m_Name.c_str());
        actorItem.put("ClassName", actorIndex->m_ClassName.c_str());

        for (auto nowTag = actorIndex->m_Tags.begin(); nowTag != actorIndex->m_Tags.end(); ++nowTag)
        {
            actorItem.add("Tag", nowTag->c_str());
        }
    }

    const boost::property_tree::xml_writer_settings<ptree::key_type> writeSettings(' ', 2);
    write_xml(projectFilePath, ptProject, std::locale(), writeSettings);

    Out::Log(pType::MESSAGE, "Save File Done");

    updateInitHeader(classItems, actorItems);

    Out::Log(pType::MESSAGE, "Generated Init Header");
}