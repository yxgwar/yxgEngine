#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "mesh.h"
#include <unordered_map>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

class Model
{
public:
    Model(std::string&& path);

    void Draw(Shader& shader);

    int textureCount = 0;
private:
    std::vector<std::unique_ptr<Mesh>> m_meshes;
    std::string m_directory;
    std::unordered_map<std::string, std::shared_ptr<Texture>> textures_loaded;
    // std::vector<std::shared_ptr<Texture>> textures_loaded;

    void loadModel(std::string& path);
    void processNode(aiNode *node, const aiScene *scene);
    std::unique_ptr<Mesh> processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};