#include "camera.h"

Camera::Camera()
{

}

Camera::~Camera()
{

}

bool Camera::Initialize(int w, int h)
{
    glm::vec3 dir;
    dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    dir.y = sin(glm::radians(pitch));
    dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(dir);
    glm::vec3 cameraPos = glm::vec3(x, y, z);

    //--Init the view and projection matrices
    //  if you will be having a moving camera the view matrix will need to more dynamic
    //  ...Like you should update it before you render more dynamic 
    //  for this project having them static will be fine
    view = glm::lookAt(cameraPos, //Eye Position
        cameraPos + cameraFront, //Focus point
        glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up

    projection = glm::perspective(glm::radians(40.f), //the FoV typically 90 degrees is good which is what this is set to
        float(w) / float(h), //Aspect Ratio, so Circles stay Circular
        0.01f, //Distance to the near plane, normally a small value like this
        100.0f); //Distance to the far plane, 
    return true;
}

void Camera::update(glm::mat4 shipMat)//int forward, int right)
{   

    /*float cameraSpeed = 0.05f;
    glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraRight = glm::cross(cameraFront, cameraUp);
    glm::vec3 cameraPos = glm::vec3(x, y, z);

    cameraPos += cameraSpeed * cameraFront * glm::vec3(forward);
    cameraPos += cameraSpeed * cameraRight * glm::vec3(right);

    view = glm::lookAt(cameraPos, cameraPos + cameraFront, up);

    x = cameraPos.x;
    y = cameraPos.y;
    z = cameraPos.z;*/
}

glm::mat4 Camera::GetProjection()
{
  return projection;
}

glm::mat4 Camera::GetView()
{
  return view;
}
