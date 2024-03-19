#include <iostream>
#include "glfont.h"

Mesh generateTextGeometryBuffer(std::string text, float characterHeightOverWidth, float totalTextWidth) {
    float characterWidth = totalTextWidth / float(text.length());
    float characterHeight = characterHeightOverWidth * characterWidth;

    unsigned int vertexCount = 4 * text.length();
    unsigned int indexCount = 6 * text.length();

    Mesh mesh;

    mesh.vertices.resize(vertexCount);
    mesh.indices.resize(indexCount);
    mesh.textureCoordinates.resize(vertexCount);

    for(unsigned int i = 0; i < text.length(); i++)
    {
        float baseXCoordinate = float(i) * characterWidth;

        mesh.vertices.at(4 * i + 0) = {baseXCoordinate, 0, 0};
        mesh.vertices.at(4 * i + 1) = {baseXCoordinate + characterWidth, 0, 0};
        mesh.vertices.at(4 * i + 2) = {baseXCoordinate + characterWidth, characterHeight, 0};

        // mesh.vertices.at(4 * i + 0) = {baseXCoordinate, 0, 0};
        // mesh.vertices.at(4 * i + 2) = {baseXCoordinate + characterWidth, characterHeight, 0};
        mesh.vertices.at(4 * i + 3) = {baseXCoordinate, characterHeight, 0};


        mesh.indices.at(6 * i + 0) = 4 * i + 0;
        mesh.indices.at(6 * i + 1) = 4 * i + 1;
        mesh.indices.at(6 * i + 2) = 4 * i + 2;
        mesh.indices.at(6 * i + 3) = 4 * i + 0;
        mesh.indices.at(6 * i + 4) = 4 * i + 2;
        mesh.indices.at(6 * i + 5) = 4 * i + 3;

        // check which character should be rendered

        // Calculate UV coordinates
        int col = static_cast<int>(text[i]);

        float left = static_cast<float>(col) / 128.0f;
        float right = static_cast<float>(col + 1) / 128.0f;
        float bottom = 0.0f; 
        float top = static_cast<float>(1);

        // map to uv coords
        mesh.textureCoordinates[4 * i]     = {left, bottom}; 
        mesh.textureCoordinates[4 * i + 1] = {right, bottom}; 
        mesh.textureCoordinates[4 * i + 2] = {right, top}; 
        mesh.textureCoordinates[4 * i + 3] = {left, top};
    }

    return mesh;
}