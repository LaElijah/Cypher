#version 330 core
out vec4 FragColor;

uniform vec2 colorValue;

void main()
{
   FragColor = vec4(colorValue.x, 0.0, 0.0, 1.0);
} 
