#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
#include <stack>
using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"
#include "sphere.h"
#include "mesh.h"
#include "skybox.h"
#include "Asteroid.h"

#define numVBOs 2;
#define numIBs 2;


class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height);
    void HierarchicalUpdate2(double dt);
    void Render();

    Camera* getCamera() { return m_camera; }
    Mesh* getShip() { return m_mesh; }

  private:
    std::string ErrorString(GLenum error);

    bool collectShPrLocs();
    void ComputeTransforms (double dt, std::vector<float> speed, std::vector<float> dist,
        std::vector<float> rotSpeed, glm::vec3 rotVector, std::vector<float> scale, 
        glm::mat4& tmat, glm::mat4& rmat, glm::mat4& smat);

    stack<glm::mat4> modelStack;

    Camera *m_camera;
    Shader *m_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
    GLint m_positionAttrib;
    GLint m_colorAttrib;
    GLint m_tcAttrib;
    GLint m_hasTexture;


    Sphere* m_sun; //done
    Sphere* m_moon; //done
    Sphere* m_earth; //done
    Sphere* m_ceres; //done
    Sphere* m_eris; //done
    Sphere* m_haumea;//done
    Sphere* m_jupiter;//done
    Sphere* m_mars;//done
    Sphere* m_mercury;//done
    Sphere* m_neptune;//done
    Sphere* m_saturn;//done
    Sphere* m_uranus;//done
    Sphere* m_venus;//done

    skybox* m_skybox;
    Shader* m_skyshader;
    Mesh* m_mesh;
    Asteroid* m_asteroid;



};

#endif /* GRAPHICS_H */
