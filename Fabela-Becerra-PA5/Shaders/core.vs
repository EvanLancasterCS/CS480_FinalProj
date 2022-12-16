#version 460
layout (location = 0) in vec3 v_position; 
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_tc;  
uniform bool hasTexture;
uniform bool hasNormal;

smooth out vec3 normal;
out vec2 tc;

layout (binding=0) uniform sampler2D normalSP;
layout (binding=1) uniform sampler2D textureSP;

uniform mat4 projectionMatrix; 
uniform mat4 viewMatrix; 
uniform mat4 modelMatrix; 

void main(void) 
{ 
    vec4 v = vec4(v_position, 1.0); 
    gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * v;
    normal = v_normal; 
    tc = v_tc;
} 