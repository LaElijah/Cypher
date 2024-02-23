#version 330 core
out vec4 FragColor;

in vec3 colorValue;

void main()
{
   FragColor = vec4(colorValue, 1.0);    
} 
