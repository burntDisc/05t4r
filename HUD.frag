#version 330

out vec4 fragColor;
in vec3 vertColor;
in vec4 gl_FragCoord;

void main()
{
    float alpha = 0.0;
    float toggleFac = mod((gl_FragCoord.x + gl_FragCoord.y), 2.0);

    if(toggleFac < 1.0)
    {
        alpha = 1.0;
    }

    fragColor = vec4(vertColor, alpha);
}