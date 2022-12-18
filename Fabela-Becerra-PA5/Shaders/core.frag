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


smooth in vec3 normal;
layout (binding=0) uniform sampler2D normalSP;
layout (binding=1) uniform sampler2D textureSP;

in vec2 tc;
in vec3 varLdir;
in vec3 varPos;
in vec3 varNorm;
uniform bool hasTexture;
uniform bool hasNormal;

out vec4 frag_texture;
out vec4 frag_normal;
out vec4 frag_color;

void main(void) 
{
    vec3 L = normalize(varLdir);
    if(hasTexture)
        frag_texture = texture(textureSP, tc); 
    else 
        frag_texture = vec4(normal.rgb, 1.0);
    
    vec3 norm;
    if(hasNormal)
    {
        norm = normalize(normal);
        norm = normalize(varNorm + texture(normalSP, tc).rgb*2 - 1); 
        norm = normalize(norm * 2.0 - 1.0); // map from [0, 1] to [-1, 1]
    }
    else 
        norm = normalize(varNorm);
    
    vec3 N = norm;
    vec3 V = normalize(-varPos);
    vec3 R = normalize(reflect(-L, N));
    float cosTheta = dot(L, N);
    float cosPhi = dot(R, V);

    vec3 amb = ((GlobalAmbient)+(texture(textureSP,tc)*light.ambient * material.ambient)/1).xyz;
    vec3 dif = light.diffuse.xyz * material.diffuse.xyz * texture(textureSP, tc).xyz * max(0.0, cosTheta);
    vec3 spc = light.spec.xyz * material.spec.xyz * pow(max(0.0,cosPhi),material.shininess);

    frag_texture = vec4(amb+dif+spc, 1);
} 