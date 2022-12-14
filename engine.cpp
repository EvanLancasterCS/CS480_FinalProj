

#include "engine.h"
#include "glm/ext.hpp"

float Engine::lastX = 400;
float Engine::lastY = 400;
Engine* Engine::instance = nullptr;

Engine::Engine(const char* name, int width, int height)
{
  m_WINDOW_NAME = name;
  m_WINDOW_WIDTH = width;
  m_WINDOW_HEIGHT = height;

  instance = this;
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

  glfwSetCursorPosCallback(m_window->getWindow(), cursorPositionCallBack);


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
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window->getWindow(), true);


    // Update camera animation here.
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window->getWindow(), true);
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_W) == GLFW_PRESS)
        m_graphics->getCamera()->update(1, 0);
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_A) == GLFW_PRESS)
        m_graphics->getCamera()->update(0, -1);
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_S) == GLFW_PRESS)
        m_graphics->getCamera()->update(-1, 0);
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_D) == GLFW_PRESS)
        m_graphics->getCamera()->update(0, 1);




}

void Engine::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (Engine::instance->first)
    {
        Engine::lastX = xpos;
        Engine::lastY = ypos;
        Engine::instance->first = false;
    }

    float xOff = xpos - Engine::lastX;
    float yOff = ypos - Engine::lastY;
    Engine::lastX = xpos;
    Engine::lastY = ypos;

    const float sens = 0.1f;
    xOff *= sens;
    yOff *= sens;

    float yaw = Engine::instance->m_graphics->getCamera()->yaw;
    float pitch = Engine::instance->m_graphics->getCamera()->pitch;

    yaw += xOff;
    pitch -= yOff;

    if (pitch > 89.f) pitch = 89.f;
    if (pitch < -89.f) pitch = -89.f;

    Engine::instance->m_graphics->getCamera()->yaw = yaw;
    Engine::instance->m_graphics->getCamera()->pitch = pitch;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    Engine::instance->m_graphics->getCamera()->cameraFront = glm::normalize(direction);

    Engine::instance->m_graphics->getCamera()->update(0, 0);
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
    Engine::instance->cursor_position_callback(window, xpos, ypos);
}
