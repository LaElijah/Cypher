#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;



vec4 diffuse1 = texture(texture_diffuse1, TexCoords); 
vec4 specular1 = texture(texture_specular1, TexCoords); 

vec4 fragResult = diffuse1;

void main()
{ 

    if (diffuse1.x >= 0)
    {
        fragResult = fragResult * diffuse1;
    }

    if (specular1.x >= 0)
    {
        fragResult = fragResult * specular1;
    }



    FragColor = fragResult;
}
