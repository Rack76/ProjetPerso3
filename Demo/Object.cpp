#include "Object.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STBI_MAX_DIMENSIONS 16 000 000

RendererObject::RendererObject(std::string meshFilename, std::string imageFilename, float x, float y, float z,
    float mass, float cof, float cor)
{
    orientation = glm::mat4(1.0);
    position = glm::vec3(x, y, z);
    world = glm::mat4(1.0);
    textCoord = new GLfloat*;
    m_mass = mass;
    m_cof = cof;
    m_cor = cor;
    vertexPositions = loadOBJMesh(meshFilename, vertexPositionsSize, textCoord, textCoordSize, vertexPositionsVector, unsortedVertexArray, order);
    meshInfo(vertexPositionsSize / (3 * 3 * 4), faceInfoList, edgeInfoList, normals, vertexPositionsVector, unsortedVertexArray, order);
    imageData = stbi_load(imageFilename.c_str(), &imageWidth, &imageHeight, &bpp, 4);
    setUpRenderingPipelineData();
}

void RendererObject::setUpRenderingPipelineData()
{

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &textureCoordinatesName);
    glBindBuffer(GL_ARRAY_BUFFER, textureCoordinatesName);
    glBufferData(GL_ARRAY_BUFFER, textCoordSize, *textCoord, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, (void*) 0);
    glEnableVertexAttribArray(1);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
    glActiveTexture(GL_TEXTURE0);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexPositionsSize, vertexPositions, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, (void*)0);
    glEnableVertexAttribArray(0);
}

void RendererObject::bind()
{
    glBindVertexArray(vao);
}

void RendererObject::update()
{
    world = orientation * glm::mat4(1.0, 0.0, 0.0, position.x,
                                    0.0, 1.0, 0.0, position.y,
                                    0.0, 0.0, 1.0, position.z,
                                    0.0, 0.0, 0.0, 1.0);
}

RendererObject::~RendererObject()
{
    glDeleteTextures(1, &texture);
    stbi_image_free(imageData);
    delete[] vertexPositions;
    delete[] textCoord;
    delete[] imageData;
}