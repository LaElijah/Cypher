#version 460 core
#extension GL_ARB_bindless_texture : require


out vec4 FragColor;
flat in uint draw;
in vec2 TexCoords;
in vec3 Test;
in float Tester;

layout(binding = 0, std430) buffer textureBuffer
{
    sampler2D textures[];
};



uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;


vec4 diffuse1 = texture(textures[draw], TexCoords); 


vec3 TestEnd = Test;

void main()
{ 
    FragColor = diffuse1;
}
