#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "renderer/openGL/vertexarray.h"
#include "renderer/openGL/indexbuffer.h"
#include "renderer/openGL/vertexbuffer.h"
#include "renderer/openGL/shader.h"
#include "renderer/openGL/texture.h"

#include <vector>
#include <string>
#include <memory>

#define MAX_BONE_INFLUENCE 4

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
	//bone indexes which will influence this vertex
	int m_BoneIDs[MAX_BONE_INFLUENCE];
	//weights from each bone
	float m_Weights[MAX_BONE_INFLUENCE];
};

class Mesh
{
public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture>> textures, float shininess);

    void Draw(Shader &shader);
private:
    void setupMesh();
private:
    std::vector<Vertex>       m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<std::shared_ptr<Texture>>      m_textures;

    VertexBuffer VBO;
    IndexBuffer EBO;
    VertexArray VAO;

    float m_shininess;
};