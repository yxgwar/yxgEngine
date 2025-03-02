#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "renderer/openGL/vertexarray.h"
#include "renderer/openGL/indexbuffer.h"
#include "renderer/openGL/vertexbuffer.h"

struct Vertex
{
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
};

class Mesh
{
public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
    Mesh(float* vertices, unsigned int* indices, size_t vsize, size_t isize);
    ~Mesh() = default;

    void Draw();
private:
    VertexBuffer VBO;
    IndexBuffer EBO;
    VertexArray VAO;
};