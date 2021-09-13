#pragma once

#include"MathAIO.h"
#include"ToolAIO.h"

namespace Loader
{
    class Vertex
    {
    public:
        Vertex() = default;
        ~Vertex() = default;

    public:
        Point m_Location;
        Direction m_Normal;
        TextureCoords m_TextureCoords;
    };

    class Mesh
    {
    public:
        Mesh() = default;
        ~Mesh() = default;

    public:
        std::vector<std::shared_ptr<Vertex>> Vertices;
        std::vector<std::shared_ptr<TriangleFace>> Faces;
    };

    class ModelLoader
    {
    public:
        ModelLoader()
        {
            Loader = std::make_unique<Assimp::Importer>();
        }
        virtual ~ModelLoader()
        {
            Clear();
        }

    public:
        std::unique_ptr<Assimp::Importer> Loader;
        std::vector<std::shared_ptr<Mesh>> m_Meshs;

    public:
        bool Load(const char* filePath)
        {
            const aiScene* scene = Loader->ReadFile(filePath, aiProcess_Triangulate | aiProcess_GenNormals);

            //Error
            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
            {
                return false;
            }

            ProcessNode(scene->mRootNode, scene);

            return true;
        }

    private:
        void ProcessMesh(aiMesh* mesh, const aiScene* scene)
        {
            auto ThisMesh = std::make_shared<Mesh>();

            //Mesh Name
            for (uint32_t i = 0; i < mesh->mNumVertices; i += 1)
            {
                auto ThisVertex = std::make_shared<Vertex>();

                ThisVertex->m_Location.x() = mesh->mVertices[i].x;
                ThisVertex->m_Location.y() = mesh->mVertices[i].y;
                ThisVertex->m_Location.z() = mesh->mVertices[i].z;

                ThisVertex->m_Normal.x() = mesh->mNormals[i].x;
                ThisVertex->m_Normal.y() = mesh->mNormals[i].y;
                ThisVertex->m_Normal.z() = mesh->mNormals[i].z;

                if (mesh->mTextureCoords[0])
                {
                    ThisVertex->m_TextureCoords.x() = mesh->mTextureCoords[0][i].x;
                    ThisVertex->m_TextureCoords.y() = mesh->mTextureCoords[0][i].y;
                }
                else
                {
                    ThisVertex->m_TextureCoords.x() = 0;
                    ThisVertex->m_TextureCoords.y() = 0;
                }

                ThisMesh->Vertices.push_back(ThisVertex);
            }

            for (uint32_t i = 0; i < mesh->mNumFaces; i += 1)
            {
                if (mesh->mFaces[i].mNumIndices != 3) continue;

                auto ThisFace = std::make_shared<TriangleFace>();
                ThisFace->x() = mesh->mFaces[i].mIndices[0];
                ThisFace->y() = mesh->mFaces[i].mIndices[1];
                ThisFace->z() = mesh->mFaces[i].mIndices[2];

                ThisMesh->Faces.push_back(ThisFace);
            }

            m_Meshs.push_back(ThisMesh);
        }

        void ProcessNode(aiNode* node, const aiScene* scene)
        {
            for (uint32_t i = 0; i < node->mNumMeshes; i += 1)
            {
                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
                ProcessMesh(mesh, scene);
            }

            for (uint32_t i = 0; i < node->mNumChildren; i += 1)
            {
                ProcessNode(node->mChildren[i], scene);
            }
        }

        void Clear()
        {
            m_Meshs.clear();
        }

    };
};