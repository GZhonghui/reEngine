#pragma once

#include"ToolAIO.h"
#include"MathAIO.h"

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
        fprintf(headerFile, "\n");
        fprintf(headerFile, "class %s : public Actor\n", className);
        fprintf(headerFile, "{\n");
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

    boost::property_tree::xml_writer_settings<ptree::key_type> writeSettings(' ', 2);
    write_xml("Engine.vcxproj", ptProject, std::locale(), writeSettings);
}

inline void updateInitHeader()
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

        fprintf(initHeader, "\n");
        fprintf(initHeader, "inline void initScene(std::vector<std::shared_ptr<Actor>>* actorsInScene)\n");
        fprintf(initHeader, "{\n");
        fprintf(initHeader, "\n");

        //For Each

        fprintf(initHeader, "\n");
        fprintf(initHeader, "}\n");
        fprintf(initHeader, "\n");
    }
}

inline void readProject()
{

}

inline void saveProject()
{
    
}