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


layout(binding = 0, std430) buffer transformBudder
{
    mat4 positions[];
};


out uint draw;

void main()
{
    TexCoords = aTexCoords;    
    gl_Position = projection * view * model * positions[gl_DrawID] * vec4(aPos, 1.0);
    draw = gl_DrawID;
}
