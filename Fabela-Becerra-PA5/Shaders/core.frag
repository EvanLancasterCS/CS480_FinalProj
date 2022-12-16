#version 460

smooth in vec3 normal;
layout (binding=0) uniform sampler2D normalSP;
layout (binding=1) uniform sampler2D textureSP;

in vec2 tc;
uniform bool hasTexture;
uniform bool hasNormal;

out vec4 frag_texture;
out vec4 frag_normal;

void main(void) 
{
    if(hasTexture)
        frag_texture = texture(textureSP, tc); 
    else 
        frag_texture = vec4(normal.rgb, 1.0);
    
    if(hasNormal)
    {
        vec3 norm = normalize(normal);
        norm = texture(normalSP, tc).rgb;
        frag_normal = vec4(normalize(norm * 2.0 - 1.0), 1.0); // map from [0, 1] to [-1, 1]
    }
    else 
        frag_normal = vec4(normal.rgb, 1.0);
} 