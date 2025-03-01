#include "import.h"
#include "object/component.h"

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

void Import::GenDefaultMaterial()
{
    ShaderPool["default"] = std::make_shared<Shader>("../assets/shaders/default/default.vs", "../assets/shaders/default/default.fs");
}

std::shared_ptr<Texture> Import::LoadTexture(std::string &path)
{
    if(TexturePool.find(path) == TexturePool.end())
    {
        TexturePool[path] = std::make_shared<Texture>(path);
        return TexturePool[path];
    }
    else
        return TexturePool[path];
}

void Import::LoadModel(std::string &path, std::vector<std::shared_ptr<MeshSlot>>& meshes, std::vector<std::shared_ptr<Material>>& materials)
{
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