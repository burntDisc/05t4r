#version 330

out vec4 fragColor;
in vec3 vertColor;

void main()
{
    fragColor = vec4(vertColor, 1.0);
}