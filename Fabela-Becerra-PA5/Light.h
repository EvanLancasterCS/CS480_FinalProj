#ifndef LIGHT_H
#define LIGHT_H

#include "graphics_headers.h"

class Light
{
public:
	Light();
	Light(glm::mat4 viewMatrix);

	GLfloat m_globalAmbient[4];
	GLfloat m_lightAmbient[4];
	GLfloat m_lightDiffuse[4];
	glm::vec3 m_lightPosition;
	GLfloat m_lightPositionViewSpace[3];
	GLfloat m_lightSpecular[4];
};



#endif
