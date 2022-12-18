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

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 v_normal;

out vec3 varNorm;
out vec3 varLdir;
out vec3 varPos;
out vec2 tc;

layout (binding=0) uniform sampler2D samp;
layout (binding=1) uniform sampler2D samp1;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat3 normMatrix;

void main(void){
	vec4 v = vec4(v_position, 1.0);
	gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * v;
	tc = texCoord;
	varPos = (viewMatrix * modelMatrix * vec4(v_position, 1.0f)).xyz;
	varLdir = light.position-varPos;
	varNorm = normMatrix*v_normal;
}
