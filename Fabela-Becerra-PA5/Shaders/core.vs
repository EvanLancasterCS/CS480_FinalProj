#version 460

struct PositionalLight{
vec4 ambient;
vec4 diffuse;
vec4 spec;
vec3 position;
};
uniform vec4 GlobalAmbient;
uniform PositionalLight light;

struct Material{
vec4 ambient;
vec4 diffuse;
vec4 spec;
float shininess;
};
uniform Material material;

layout (location = 0) in vec3 v_position; 
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_tc;  
uniform bool hasTexture;
uniform bool hasNormal;

smooth out vec3 normal;
out vec2 tc;
out vec3 varPos;
out vec3 varLdir;
out vec3 varNorm;

layout (binding=0) uniform sampler2D normalSP;
layout (binding=1) uniform sampler2D textureSP;

uniform mat4 projectionMatrix; 
uniform mat4 viewMatrix; 
uniform mat4 modelMatrix; 
uniform mat3 normMatrix;


void main(void) 
{ 
    vec4 v = vec4(v_position, 1.0); 
    varPos = (viewMatrix * modelMatrix * vec4(v_position,1.0f)).xyz;
    varPos = (modelMatrix * v).xyz;

    varLdir = light.position-varPos;
    normal = v_normal; 
    tc = v_tc;
    gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * v;
    gl_Position = (projectionMatrix * viewMatrix * vec4(varPos, 1.0));
    
    varNorm = normMatrix * v_normal;
    varNorm = mat3(transpose(inverse(modelMatrix))) * v_normal;

} 