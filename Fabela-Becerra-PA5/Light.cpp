#include "Light.h"

Light::Light()
{

}
Light::Light(glm::mat4 viewMatrix)
{
	m_lightPosition = glm::vec3(0, 0, 0);

	m_lightPositionViewSpace[0] = glm::vec4((viewMatrix * glm::vec4(m_lightPosition, 1.0f))).x;
	m_lightPositionViewSpace[1] = glm::vec4((viewMatrix * glm::vec4(m_lightPosition, 1.0f))).y;
	m_lightPositionViewSpace[2] = glm::vec4((viewMatrix * glm::vec4(m_lightPosition, 1.0f))).z;


	m_lightAmbient[0] = 0.9;
	m_lightAmbient[1] = 0.4;
	m_lightAmbient[2] = 0.0;
	m_lightAmbient[3] = 1;
	m_lightDiffuse[0] = 0.7;
	m_lightDiffuse[1] = 0.7;
	m_lightDiffuse[2] = 0.7;
	m_lightDiffuse[3] = 1;
	m_lightSpecular[0] = 1;
	m_lightSpecular[1] = 1;
	m_lightSpecular[2] = 1;
	m_lightSpecular[3] = 1;
	m_globalAmbient[0] = 0.1;
	m_globalAmbient[1] = 0.1;
	m_globalAmbient[2] = 0.2;
	m_globalAmbient[3] = 0.1;
}