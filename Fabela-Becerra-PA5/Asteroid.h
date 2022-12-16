#ifndef ASTEROID_H
#define ASTEROID_H

#include <vector>
#include "graphics_headers.h"
#include "Texture.h"
#include "shader.h"
#include "camera.h"

class Asteroid
{
public:
    Asteroid();
    ~Asteroid();
    Asteroid(glm::vec3 pivot, const char* fname);
    Asteroid(glm::vec3 pivot, const char* fname, const char* tname);

    void Update(glm::mat4 model, double dt);
    void Render(GLint posAttribLoc, GLint colAttribLoc, GLint tcAttribLoc, GLint hasTextureLoc, GLint hasNormalLoc);
    void Render(Shader* m_shader, Camera* m_camera);

    glm::mat4 GetModel();
    glm::mat4 GetPosition() { return positionMatrix; }

    bool InitBuffers();
    bool loadModelFromFile(const char* path);
    bool ShaderInfo(Shader* shader);

    bool hasTexture;
    bool hasNormal;
    GLuint getTextureID() { return m_texture->getTextureID(); }
    GLuint getNormalTextureID() { return m_normalTexture->getTextureID(); }

    void SetSpeed(std::vector<double> newSpeed) { speed = newSpeed; }
    void SetDist(std::vector<double> newDist) { dist = newDist; }
    void SetRotSpeed(float newRotSpeed) { rotSpeed = newRotSpeed; }
    void SetScale(std::vector<double> newScale) { scale = newScale; }
    void SetRotVector(glm::vec3 newRotVector) { rotVector = newRotVector; }

private:
    glm::vec3 pivotLocation;
    glm::mat4 model;
    glm::mat4 positionMatrix;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;

    Texture* m_texture;
    Texture* m_normalTexture{};

    GLuint vao;

    float angle;

    // Position Variables
    float rotSpeed;
    glm::vec3 rotVector;
    std::vector<double> speed;
    std::vector<double> dist;
    std::vector<double> scale;

    // Shader Variables
    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
    GLint m_positionAttrib;
    GLint m_colorAttrib;
    GLint m_tcAttrib;
    GLint m_hasTexture;
    GLint m_hasNormal;
};

#endif