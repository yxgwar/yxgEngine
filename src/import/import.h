#pragma once

#include "renderer/openGL/texture.h"
#include "renderer/openGL/shader.h"
#include "object/material.h"
#include "object/mesh.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <memory>

struct MeshSlot;

#define PATH "../assets/"
#define SHADERPATH "../assets/shaders/"

class Import
{
public:
    static void GenDefault();

    static std::shared_ptr<Texture> LoadTexture(std::string& path);

    static void LoadModel(std::string& path, std::vector<std::shared_ptr<MeshSlot>>& meshes, std::vector<std::shared_ptr<Material>>& materials);

    static void LoadShader(std::string& vertexPath, std::string& fragmentPath, std::string& geometryPath);
public:
    // 全局纹理资源池
    inline static std::unordered_map<std::string, std::shared_ptr<Texture>> TexturePool;
    inline static std::unordered_set<std::string> texturePaths;

    // 着色器
    inline static std::unordered_map<std::string, std::shared_ptr<Shader>> ShaderPool;

    // 材质 mesh (暂未启用)
    inline static std::unordered_map<std::string, std::shared_ptr<Material>> MaterialPool;
    inline static std::unordered_map<std::string, std::vector<std::shared_ptr<MeshSlot>>> MeshPool;
private:
    static void GenStandardQuad();
    static void GenStandardCube();
};