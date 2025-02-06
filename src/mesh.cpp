#include "mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
    :m_vertices(std::move(vertices)), m_indices(std::move(indices)), m_textures(std::move(textures)),
    VBO((float*)&m_vertices[0], m_vertices.size() * sizeof(Vertex)),
    EBO(&m_indices[0], m_indices.size() * sizeof(unsigned int))
{
    setupMesh();
}

void Mesh::Draw(Shader &shader)
{
    // bind appropriate textures
    unsigned int diffuseNr  = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr   = 1;
    unsigned int heightNr   = 1;

    for(unsigned int i = 0; i < m_textures.size(); i++)
    {
        std::string number;
        std::string name = m_textures[i].getType();
        if(name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if(name == "texture_specular")
            number = std::to_string(specularNr++); // transfer unsigned int to string
        else if(name == "texture_normal")
            number = std::to_string(normalNr++); // transfer unsigned int to string
            else if(name == "texture_height")
            number = std::to_string(heightNr++); // transfer unsigned int to string
        m_textures[i].bind(i);
        shader.setInt((name + number).c_str(), i);
    }

    VAO.Draw();
    glActiveTexture(GL_TEXTURE0);
}

void Mesh::setupMesh()
{
    std::vector<VertexAttribute> attributes = {
        {0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0},
        {1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal)},
        {2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords)},
        {3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent)},
        {4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent)},
        {5, 4, GL_INT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs)},
        {6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights)}
    };

    VAO.AddVBO(VBO, attributes);
    VAO.SetEBO(EBO, sizeof(m_indices), GL_UNSIGNED_INT);
}
