#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;
layout (location = 2) in vec2 aTexCoord;

out vec3 colorValue;
out vec2 TexCoord;

uniform vec2 offset;

void main()
{
  colorValue = aCol;
  gl_Position = vec4(aPos.x + offset.x, aPos.y + offset.y, aPos.z, 1.0);
  TexCoord = aTexCoord;
}

