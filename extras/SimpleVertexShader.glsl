#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;

uniform vec2 offset;

out vec3 colorValue;
void main()
{
  colorValue = aCol;
  gl_Position = vec4(aPos.x + offset.x, aPos.y + offset.y, aPos.z, 1.0);
}

