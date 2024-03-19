#include <glad/glad.h>
#include <program.hpp>
#include "glutils.h"
#include <vector>

template <class T>
unsigned int generateAttribute(int id, int elementsPerEntry, std::vector<T> data, bool normalize) {
    unsigned int bufferID;
    glGenBuffers(1, &bufferID);
    glBindBuffer(GL_ARRAY_BUFFER, bufferID);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(T), data.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(id, elementsPerEntry, GL_FLOAT, normalize ? GL_TRUE : GL_FALSE, sizeof(T), 0);
    glEnableVertexAttribArray(id);
    return bufferID;
}

unsigned int generateBuffer(Mesh &mesh) {
    unsigned int vaoID;
    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);
    
    generateAttribute(0, 3, mesh.vertices, false);
    if (mesh.normals.size() > 0) {
        generateAttribute(1, 3, mesh.normals, true);
    }
    if (mesh.textureCoordinates.size() > 0) {
        generateAttribute(2, 2, mesh.textureCoordinates, false);
    }
    
    // Calculate tangents and bitangents
    std::vector<glm::vec3> tangents(mesh.vertices.size(), glm::vec3(0.0f));
    std::vector<glm::vec3> bitangents(mesh.vertices.size(), glm::vec3(0.0f));
    for (size_t i = 0; i < mesh.indices.size(); i += 3) {
        unsigned int index0 = mesh.indices[i];
        unsigned int index1 = mesh.indices[i + 1];
        unsigned int index2 = mesh.indices[i + 2];

        glm::vec3 edge1 = mesh.vertices[index1] - mesh.vertices[index0];
        glm::vec3 edge2 = mesh.vertices[index2] - mesh.vertices[index0];

        glm::vec2 deltaUV1 = mesh.textureCoordinates[index1] - mesh.textureCoordinates[index0];
        glm::vec2 deltaUV2 = mesh.textureCoordinates[index2] - mesh.textureCoordinates[index0];

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        glm::vec3 tangent;
        tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

        glm::vec3 bitangent;
        bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

        tangents[index0] += tangent;
        tangents[index1] += tangent;
        tangents[index2] += tangent;

        bitangents[index0] += bitangent;
        bitangents[index1] += bitangent;
        bitangents[index2] += bitangent;
    }

    // Normalize tangents and bitangents
    for (size_t i = 0; i < mesh.vertices.size(); ++i) {
        mesh.tangents.push_back(glm::normalize(tangents[i]));
        mesh.bitangents.push_back(glm::normalize(bitangents[i]));
    }

    generateAttribute(3, 3, mesh.tangents, false);
    generateAttribute(4, 3, mesh.bitangents, false);

    unsigned int indexBufferID;
    glGenBuffers(1, &indexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), mesh.indices.data(), GL_STATIC_DRAW);

    return vaoID;
}
