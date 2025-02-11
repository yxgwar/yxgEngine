#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "mesh.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

class Model
{
public:
    Model(std::string&& path);

    void Draw(Shader& shader);

    void SetPosition(glm::vec3 pos);

    inline glm::mat4 getModel() {return model;}
    inline glm::mat4 getNormalM() {return NormalM;}
private:
    std::vector<std::unique_ptr<Mesh>> m_meshes;
    std::string m_directory;
    std::vector<std::shared_ptr<Texture>> textures_loaded;

    void loadModel(std::string& path);
    void processNode(aiNode *node, const aiScene *scene);
    std::unique_ptr<Mesh> processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

    glm::mat4 model;
    glm::mat4 NormalM;
};