#include "camera.h"

Camera::Camera()
{
    
}

Camera::~Camera()
{
    
}

void Camera::Update(glm::vec3 focus, glm::vec3 frontVec, glm::vec3 upVec, float dt)
{
    if (currentMode == Exploration) {
        // we care about all parameters here
        glm::vec3 cameraPosition = focus - frontVec * 1.5f + upVec * 0.5f;
        glm::vec3 focusPoint = focus + frontVec * 1.f;

        cameraUp = upVec;

        view = glm::lookAt(cameraPosition, focusPoint, cameraUp);
        cameraPos = cameraPosition;

        projection = glm::perspective(glm::radians(fov), aspect, 0.01f, 100.0f);
    }
    else if (currentMode == Observation)
    {
        // we care only about the focus, handle front based on player input, upvec is 0, 1, 0
        cameraUp = glm::vec3(0, 1, 0);
        glm::vec3 focusOffset = glm::vec3(0, 0, 0);
        float x = OBSERVATION_DISTANCE;
        float z = 0;
        if (OBSERVATION_ORBIT_SPEED != 0) {
            x = glm::cos(dt / OBSERVATION_ORBIT_SPEED) * OBSERVATION_DISTANCE;
            z = glm::sin(dt / OBSERVATION_ORBIT_SPEED) * OBSERVATION_DISTANCE;
            focusOffset = glm::vec3(glm::sin(dt / OBSERVATION_ORBIT_SPEED) * observationOffsetX, observationOffsetY, -glm::cos(dt / OBSERVATION_ORBIT_SPEED) * observationOffsetX);
        }

        glm::vec3 cameraPosition = focus + glm::vec3(x, 0, z);
        cameraPos = cameraPosition;

        view = glm::lookAt(cameraPosition, focus + focusOffset, cameraUp);

        projection = glm::perspective(glm::radians(fov), aspect, 0.01f, 100.0f);
    }
}

bool Camera::Initialize(int w, int h)
{
 
  aspect = float(w) / float(h);
  view = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);

  projection = glm::perspective( glm::radians(fov), //the FoV typically 90 degrees is good which is what this is set to
                                 aspect, //Aspect Ratio, so Circles stay Circular
                                 0.01f, //Distance to the near plane, normally a small value like this
                                 100.0f); //Distance to the far plane, 

  UpdateCameraVectors();
  return true;
}

glm::mat4 Camera::GetProjection()
{
  return projection;
}

glm::mat4 Camera::GetView()
{
  return view;
}


void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    //float velocity = MovementSpeed * deltaTime;
    switch (direction)
    {
    case TOGGLE_MODE:
        if (currentMode == Observation)
            currentMode = Exploration;
        else
            currentMode = Observation;
        break;
    case FORWARD:
        observationIndex++;
        break;
    case BACKWARD:
        observationIndex--;
        break;
    default:
        break;
        /*
    case FORWARD:
        cameraPos += cameraFront * velocity;
        break;
    case BACKWARD:
        cameraPos -= cameraFront * velocity;
        break;
    case LEFT:
        cameraPos -= cameraRight * velocity;
        break;
    case RIGHT:
        cameraPos += cameraRight * velocity;
        break;
    default:
        break;
        */
    }
    //UpdateCameraVectors();
}


void Camera::ProcessMouseMovement(float xoffset, float yoffset)
{
    if (currentMode == Observation) {
        xoffset *= OBSERVATION_SENSITIVITY;
        yoffset *= OBSERVATION_SENSITIVITY;

        observationOffsetX += xoffset;
        observationOffsetY += yoffset;

        if (sqrt(observationOffsetX * observationOffsetX + observationOffsetY * observationOffsetY) > OBSERVATION_BOUNDS)
        {
            glm::vec2 helperVec = glm::vec2(observationOffsetX, observationOffsetY);
            helperVec = glm::normalize(helperVec) * (OBSERVATION_BOUNDS - 0.001f);
            

            observationOffsetX = helperVec.x;
            observationOffsetY = helperVec.y;
        }
        /*
        if (observationOffsetX < -OBSERVATION_BOUNDS)
            observationOffsetX = -OBSERVATION_BOUNDS;
        else if (observationOffsetX > OBSERVATION_BOUNDS)
            observationOffsetX = OBSERVATION_BOUNDS;
        if (observationOffsetY < -OBSERVATION_BOUNDS)
            observationOffsetY = -OBSERVATION_BOUNDS;
        else if (observationOffsetY > OBSERVATION_BOUNDS)
            observationOffsetY = OBSERVATION_BOUNDS;
        */
        /*Yaw += xoffset;
        Pitch += yoffset;

        // Bounds checking
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;*/

        //UpdateCameraVectors();
    }
}

void Camera::UpdateCameraVectors()
{
    // calculate the new Front vector
    
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    cameraFront = glm::normalize(front);

    cameraRight = glm::normalize(glm::cross(cameraFront, up));
    cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
    //Update();
}

void Camera::ProcessScrollMovement(float yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.f)
        fov = 45.0f;

   //UpdateCameraVectors();
}