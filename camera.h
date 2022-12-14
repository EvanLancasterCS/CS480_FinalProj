#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"

class Camera
{
  public:
    Camera();
    ~Camera();
    bool Initialize(int w, int h);
    glm::mat4 GetProjection();
    glm::mat4 GetView();

    void update(int forward, int right);

    float yaw = 90.f;
    float pitch = -45.f;
  
    glm::vec3 cameraFront = glm::vec3(0.f, 0.f, 0.f);

  private:

    double x = 0.0;
    double y = 10.0;
    double z = -16.0;
    glm::mat4 projection;
    glm::mat4 view;
};

#endif /* CAMERA_H */
