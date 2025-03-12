#include "import.h"
#include "object/component.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <map>
#include <iostream>

void loadMatTexture(aiMaterial* aiMat, std::shared_ptr<Material> mat, std::string& path)
{
    // 定义纹理类型映射
    const static std::map<aiTextureType, std::string> typeMap = {
        {aiTextureType_DIFFUSE,  "texture_diffuse"},
        {aiTextureType_SPECULAR, "texture_specular"},
        {aiTextureType_NORMALS, "texture_normal"}
    };

    // 遍历所有支持的纹理类型
    for (const auto& [aiType, shaderType] : typeMap)
    {
        unsigned textureCount = aiMat->GetTextureCount(aiType);
        for (unsigned i = 0; i < textureCount; i++)
        {
            aiString aiPath;
            aiMat->GetTexture(aiType, i, &aiPath);

            // 构造完整路径并加载纹理
            std::string fullpath = path + aiPath.C_Str();
            Import::LoadTexture(fullpath);

            // 将纹理添加到材质
            mat->AddTexture(Import::LoadTexture(fullpath), shaderType, fullpath);
        }
    }
}

std::shared_ptr<MeshSlot> processMesh(aiMesh *mesh, int index)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }
        // texture coordinates
        if(mesh->HasTextureCoords(0)) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x; 
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        if(mesh->HasTangentsAndBitangents())
        {
            // tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;
            // bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
        }
        vertices.emplace_back(vertex);
    }

    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.emplace_back(face.mIndices[j]);
    }

    return std::make_shared<MeshSlot>(
        std::make_shared<Mesh>(vertices, indices),
        index
    );
}

void Import::GenDefault()
{
    // gBuffer
    ShaderPool["gBuffer"] = std::make_shared<Shader>("../assets/shaders/gBuffer/gBuffer.vs", "../assets/shaders/gBuffer/gBuffer.fs");

    // deffer light
    ShaderPool["deffer"] = std::make_shared<Shader>("../assets/shaders/deffer/deffer.vs", "../assets/shaders/deffer/deffer.fs");

    // 默认渲染材质（支持简单的diffuse贴图）
    // ShaderPool["default"] = std::make_shared<Shader>("../assets/shaders/default/default.vs", "../assets/shaders/default/default.fs");
    ShaderPool["default"] = std::make_shared<Shader>("../assets/shaders/shadow/shadow.vs", "../assets/shaders/shadow/shadow.fs");
    MaterialPool["default"] = std::make_shared<Material>(ShaderPool["default"]);
    // 生成标准矩形与立方体
    GenStandardQuad();
    GenStandardCube();

    // 生成全白纹理
    TexturePool["white"] = std::make_shared<Texture>();

    // 深度渲染材质
    ShaderPool["depthMap"] = std::make_shared<Shader>("../assets/shaders/depthMap/depthMap.vs", "../assets/shaders/depthMap/depthMap.fs");
    MaterialPool["depthMap"] = std::make_shared<Material>(ShaderPool["depthMap"]);

    // 后处理
    ShaderPool["PostProcess"] = std::make_shared<Shader>("../assets/shaders/PostProcess/PostProcess.vs", "../assets/shaders/PostProcess/PostProcess.fs");
    ShaderPool["HDR"] = std::make_shared<Shader>("../assets/shaders/HDR/hdr.vs", "../assets/shaders/HDR/hdr.fs");
    ShaderPool["blur"] = std::make_shared<Shader>("../assets/shaders/HDR/blur.vs", "../assets/shaders/HDR/blur.fs");

    // ssao
    ShaderPool["ssao"] = std::make_shared<Shader>("../assets/shaders/ssao/ssao.vs", "../assets/shaders/ssao/ssao.fs");
    ShaderPool["ssaoblur"] = std::make_shared<Shader>("../assets/shaders/ssao/ssao.vs", "../assets/shaders/ssao/ssaoblur.fs");
}

std::shared_ptr<Texture> Import::LoadTexture(std::string &path)
{
    if(TexturePool.find(path) == TexturePool.end())
    {
        std::cout << "load texture:" << path << std::endl;
        TexturePool[path] = std::make_shared<Texture>(path);
        return TexturePool[path];
    }
    else
        return TexturePool[path];
}

void Import::LoadModel(std::string &path, std::vector<std::shared_ptr<MeshSlot>>& meshes, std::vector<std::shared_ptr<Material>>& materials)
{
    std::cout << "load model:" << path << std::endl;
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }
    // retrieve the directory path of the filepath
    std::string m_directory = path.substr(0, path.find_last_of('/')) + '/';

    std::unordered_map<unsigned int, int> loadedMat;
    int matIndex = 0;

    // 遍历所有网格
    for (unsigned i = 0; i < scene->mNumMeshes; i++)
    {
        aiMesh* aiMesh = scene->mMeshes[i];
        unsigned int mindex = aiMesh->mMaterialIndex;
        if(loadedMat.find(mindex) == loadedMat.end())
        {
            aiMaterial* aiMat = scene->mMaterials[mindex];
            loadedMat[mindex] = matIndex++;

            // 创建新材质
            auto material = std::make_shared<Material>(ShaderPool["default"]);
            // 加载该材质的所有纹理
            loadMatTexture(aiMat, material, m_directory);

            materials.emplace_back(material);
        }

        meshes.emplace_back(processMesh(aiMesh, loadedMat[mindex]));
    }
}

void Import::LoadShader(std::string &vertexPath, std::string &fragmentPath, std::string &geometryPath)
{
    std::string path = vertexPath.substr(0, vertexPath.find_last_of('/'));
    if(ShaderPool.find(path) == ShaderPool.end())
    {
        if(geometryPath.empty())
            ShaderPool[path] = std::make_shared<Shader>(SHADERPATH + vertexPath, SHADERPATH + fragmentPath);
        else
            ShaderPool[path] = std::make_shared<Shader>(SHADERPATH + vertexPath, SHADERPATH + fragmentPath, SHADERPATH + geometryPath);
    }
}

std::shared_ptr<Shader> Import::GetShader(const char *name)
{
    if(ShaderPool.find(name) != ShaderPool.end())
        return ShaderPool[name];
    else
        return nullptr;
}

void Import::GenStandardQuad()
{
    float vertices[] = {
         0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // 右上角
         0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // 右下角
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // 左下角
        -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f   // 左上角
    };

    unsigned int indices[] = {
        0, 2, 1, // 第一个三角形
        0, 3, 2  // 第二个三角形
    };
    MeshPool["quad"].emplace_back(std::make_shared<MeshSlot>(std::make_shared<Mesh>(vertices, indices, sizeof(vertices), sizeof(indices)), 0));
}

void Import::GenStandardCube()
{
    float vertices[] = {
        // 前面（Z+）
        -0.5f,  0.5f, 0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f, // 0
         0.5f,  0.5f, 0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f, // 1
         0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f, // 2
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, // 3
      
        // 后面（Z-）
         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, // 4
         0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // 5
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // 6
        -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // 7
      
        // 右面（X+）
         0.5f,  0.5f, 0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f, // 8
         0.5f, -0.5f, 0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, // 9
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // 10
         0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  0.0f, 1.0f, // 11
      
        // 左面（X-）
        -0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 12
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // 13
        -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 14
        -0.5f,  0.5f, 0.5f,  -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // 15
      
        // 顶面（Y+）
        -0.5f, 0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // 16
         0.5f, 0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // 17
         0.5f, 0.5f, 0.5f,   0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // 18
        -0.5f, 0.5f, 0.5f,   0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // 19
      
        // 底面（Y-）
        -0.5f, -0.5f, 0.5f,  0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // 20
         0.5f, -0.5f, 0.5f,  0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // 21
         0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // 22
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // 23
      };

    unsigned int indices[] = {
        // 前面
        0, 2, 1,  0, 3, 2,
        // 后面
        4, 5, 6,  4, 6, 7,
        // 右面
        8, 9, 10, 8, 10, 11,
        // 左面
        12, 13, 14, 12, 14, 15,
        // 顶面
        16, 18, 17, 16, 18, 18,
        // 底面
        20, 22, 21, 20, 23, 22
    };

    MeshPool["cube"].emplace_back(std::make_shared<MeshSlot>(std::make_shared<Mesh>(vertices, indices, sizeof(vertices), sizeof(indices)), 0));
}
