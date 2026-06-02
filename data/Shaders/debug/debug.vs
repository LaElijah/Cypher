#version 460 core


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 Test;
out float Tester;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout(binding = 0, std430) buffer transformBuffer
{
    mat4 transforms[];
};


out uint draw;

void main()
{
    TexCoords = aTexCoords;    
    gl_Position = projection * view * transforms[gl_BaseInstance + gl_InstanceID] * vec4(aPos, 1.0);
    draw = gl_BaseInstance + gl_InstanceID;
}
