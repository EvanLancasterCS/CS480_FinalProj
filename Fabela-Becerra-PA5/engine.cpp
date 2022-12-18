

#include "engine.h"
#include "glm/ext.hpp"

Engine::Engine(const char* name, int width, int height)
{
  m_WINDOW_NAME = name;
  m_WINDOW_WIDTH = width;
  m_WINDOW_HEIGHT = height;

}


Engine::~Engine()
{
  delete m_window;
  delete m_graphics;
  m_window = NULL;
  m_graphics = NULL;
}

bool Engine::Initialize()
{
  // Start a window
  m_window = new Window(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT);
  if(!m_window->Initialize())
  {
    printf("The window failed to initialize.\n");
    return false;
  }

  // Start the graphics
  m_graphics = new Graphics();
  if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT))
  {
    printf("The graphics failed to initialize.\n");
    return false;
  }

  m_camera = m_graphics->getCamera();
  m_ship = m_graphics->getShip();
  glfwSetCursorPosCallback(m_window->getWindow(), cursorPositionCallBack);
  glfwSetInputMode(m_window->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetScrollCallback(m_window->getWindow(), scrollwheel_callback);

  // No errors
  return true;
}

void Engine::Run()
{
  m_running = true;

  while (!glfwWindowShouldClose(m_window->getWindow()))
  {
      ProcessInput();
      Display(m_window->getWindow(), glfwGetTime());
      glfwPollEvents();
  }
  m_running = false;

}

void Engine::ProcessInput()
{
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window->getWindow(), true);

    if (m_camera->isExploration())
    {
        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_W) == GLFW_PRESS)
            m_ship->ProcessKeyboard(FORWARD, deltaTime);

        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_S) == GLFW_PRESS)
            m_ship->ProcessKeyboard(BACKWARD, deltaTime);

        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_A) == GLFW_PRESS)
            m_ship->ProcessKeyboard(LEFT, deltaTime);

        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_D) == GLFW_PRESS)
            m_ship->ProcessKeyboard(RIGHT, deltaTime);

        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_Q) == GLFW_PRESS)
            m_ship->ProcessKeyboard(ROLL_CCW, deltaTime);

        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_E) == GLFW_PRESS)
            m_ship->ProcessKeyboard(ROLL_CW, deltaTime);

        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
            m_ship->ProcessKeyboard(BRAKE, deltaTime);
    }

    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        // track time so that it doesnt switch back and forth every frame
        if (currentFrame - lastToggleTime > 0.2f) {
            m_camera->ProcessKeyboard(TOGGLE_MODE, deltaTime);
            lastToggleTime = currentFrame;
        }
    }
}

void Engine::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
 
    glfwGetCursorPos(window, &xpos, &ypos);
    std::cout << "Position: (" << xpos << ":" << ypos << ")";
}



unsigned int Engine::getDT()
{
  //long long TimeNowMillis = GetCurrentTimeMillis();
  //assert(TimeNowMillis >= m_currentTimeMillis);
  //unsigned int DeltaTimeMillis = (unsigned int)(TimeNowMillis - m_currentTimeMillis);
  //m_currentTimeMillis = TimeNowMillis;
  //return DeltaTimeMillis;
    return glfwGetTime();
}

long long Engine::GetCurrentTimeMillis()
{
  //timeval t;
  //gettimeofday(&t, NULL);
  //long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
  //return ret;
    return (long long) glfwGetTime();
}

void Engine::Display(GLFWwindow* window, double time) {

    m_graphics->Render();
    m_window->Swap();
    m_graphics->HierarchicalUpdate2(time);
}

static void cursorPositionCallBack(GLFWwindow* window, double xpos, double ypos) {
    //cout << xpos << " " << ypos << endl;
    float x = static_cast<float>(xpos);
    float y = static_cast<float>(ypos);

    if (firstMove)
    {
        lastX = x;
        lastY = y;
        firstMove = false;
    }

    float xOffset = x - lastX;
    float yOffset = lastY - y;

    lastX = x;
    lastY = y;
    if(m_camera->isExploration())
        m_ship->ProcessMouseMovement(xOffset, yOffset);
    else
        m_camera->ProcessMouseMovement(xOffset, yOffset);
}

static void scrollwheel_callback(GLFWwindow* window, double xoffset, double yoffset) {

    m_camera->ProcessScrollMovement(yoffset);
    
}
