#include "mesh.h"
#include "glad/glad.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
    :VBO(vertices.data(), vertices.size() * sizeof(Vertex)),
    EBO(indices.data(), indices.size())
{
    std::vector<VertexAttribute> attributes = {
        {0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0},
        {1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal)},
        {2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords)},
        {3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent)},
        {4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent)}
    };

    VAO.AddVBO(VBO, attributes);
    VAO.SetEBO(EBO);
}

Mesh::Mesh(float *vertices, unsigned int *indices, size_t vsize, size_t isize)
    :VBO(vertices, vsize), EBO(indices, isize)
{
    std::vector<VertexAttribute> attributes = {
        {0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0},
        {1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))},
        {2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))}
        // {3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent)},
        // {4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent)}
    };

    VAO.AddVBO(VBO, attributes);
    VAO.SetEBO(EBO);
}

void Mesh::Draw()
{
    VAO.Draw();
}