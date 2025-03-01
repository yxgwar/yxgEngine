#include "mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
    :VBO((float*)&vertices[0], vertices.size() * sizeof(Vertex)),
    EBO(&indices[0], indices.size() * sizeof(unsigned int))
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

void Mesh::Draw()
{
    VAO.Draw();
}