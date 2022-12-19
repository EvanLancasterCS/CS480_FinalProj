#pragma once
#include <vector>
#include "Texture.h"
#include "shader.h"
#include "camera.h"

class Sphere
{
public:
    Sphere();
    Sphere(Sphere* otherSphere, glm::vec3 startAngle);
    Sphere(int prec, const char* fname, const char* nname);

    bool ShaderInfo(Shader* shader);
    void Update(glm::mat4 matModel, double dt);
    void Render(Shader* m_shader, Camera* m_camera);

    // Get functions
    glm::mat4 GetModel() { return model; }
    glm::vec3 GetPositionVertex() { return posVertex; }
    glm::mat4 GetPositionMatrix() { return positionMatrix; }
    int getNumVertices() const { return numVertices; }
    int getNumIndices() const { return numIndices; }
    std::vector<int> getIndices() { return indices; }
    std::vector<glm::vec3> getVertices() { return vertices; }
    std::vector<glm::vec2> getTexCoords() { return texCoords; }
    std::vector<glm::vec3> getNormals() { return normals; }
    GLuint getTextureID() { return m_texture->getTextureID(); }
    GLuint getNormalTextureID() { return m_normalTexture->getTextureID(); }

    // Set functions
    void SetSpeed(std::vector<double> newSpeed) { speed = newSpeed; }
    void SetDist(std::vector<double> newDist);
    void SetRotSpeed(float newRotSpeed) { rotSpeed = newRotSpeed; }
    void SetScale(std::vector<double> newScale) { scale = newScale; }
    void SetRotVector(glm::vec3 newRotVector);

    // Public Variables
    bool hasTexture;
    bool hasNormal;

    float matAmbient[4] = { 0.3, 0.3, 1, 1 };
    float matDiff[4] = { 1.0, 1.0, 1.0, 1.0 };
    float matSpec[4] = { 0.1f, 0.1f, 0.1f, 1.f };
    float matShininess = 1.0;

private:
    void init(int);
    void setupVertices();
    void setupBuffers();
    static float toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

    GLuint vao;
    GLuint VB;
    GLuint IB;

    // Texture Variables
    Texture* m_texture;
    Texture* m_normalTexture;

    // Matrix variables
    glm::mat4 model;
    glm::vec3 posVertex;
    glm::mat4 positionMatrix;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;

    int numVertices{};
    int numIndices{};
    std::vector<int> indices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;

    // Position 
    glm::vec3 angle;
    float rotSpeed;
    glm::vec3 rotVector;
    std::vector<double> speed;
    std::vector<double> dist;
    std::vector<double> scale;
    double lastFrame = 0;

    // Shader Variables
    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
    GLint m_normalMatrix;
    GLint m_positionAttrib;
    GLint m_colorAttrib;
    GLint m_tcAttrib;
    GLint m_hasTexture;
    GLint m_hasNormal;
};
