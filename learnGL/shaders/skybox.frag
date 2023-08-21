#version 450 core
out vec4 FragmentColor;

in vec3 TexCoords;

uniform samplerCube _skybox;

void main()
{
    FragmentColor = texture(_skybox, TexCoords);
}
