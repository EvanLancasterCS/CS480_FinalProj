#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    ROLL_CW,
    ROLL_CCW,
    BRAKE,
    TOGGLE_MODE
};

enum Camera_Mode {
    Exploration,
    Observation
};

// Default camera values
const float YAW = -85.0f;
const float PITCH = 0.0f;
const float SPEED = 3.5f;
const float SENSITIVITY = 0.1f;
const float OBSERVATION_SENSITIVITY = 0.003f;
const float OBSERVATION_DISTANCE = 1.5f;
const float OBSERVATION_ORBIT_SPEED = 5.0f;
const float OBSERVATION_BOUNDS = .5f;

class Camera
{
  public:
    Camera();
    ~Camera();
    void Update(glm::vec3 focus, glm::vec3 frontVec, glm::vec3 upVec, float dt);
    bool Initialize(int w, int h);
    glm::mat4 GetProjection();
    glm::mat4 GetView();

    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset);
    void ProcessScrollMovement(float yoffset);

    bool isExploration() { return currentMode == Exploration; }
    bool isObservation() { return currentMode == Observation; }

    // euler Angles
    float Yaw = YAW;
    float Pitch = PITCH;

    // camera options
    float aspect = 0;
    float fov = 40.f;

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
  private:

    double x = 0.0;
    double y = 10.0;
    double z = -16.0;
    glm::mat4 projection;
    glm::mat4 view;

    const float cameraSpeed = 3.5f;
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -12.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraRight;
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    float observationOffsetX = 0.f;
    float observationOffsetY = 0.f;

    Camera_Mode currentMode = Exploration;

    void UpdateCameraVectors();
};

#endif /* CAMERA_H */
