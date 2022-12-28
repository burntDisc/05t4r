#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 shaderPosition[];
in vec3 Normal[];
in vec3 color[];
in vec2 texCoord[];
in vec4 position[];

out vec3 shaderPositionF;
out vec3 NormalF;
out vec3 colorF;
out vec2 texCoordF;

// Imports the camera matrix
uniform mat4 projection;

uniform float time;
uniform bool explode;

void main() {

    for(int i = 0; i < 3; i++)
    {

        shaderPositionF = shaderPosition[i];
        colorF = color[i];
        texCoordF = texCoord[i];
        vec3 a = vec3(gl_in[0].gl_Position - gl_in[1].gl_Position);
        vec3 b = vec3(gl_in[2].gl_Position - gl_in[1].gl_Position);
        NormalF = normalize(cross(b,a));
        
        colorF = color[i];
        texCoordF = texCoord[i];

        float speedMult = 10;
        float distMult = 0.1;

        vec3 direction = NormalF * ((sin(time*speedMult))) * distMult;

        gl_Position = explode?
            projection * (gl_in[i].gl_Position + vec4(direction, 0.0)) :
            projection * gl_in[i].gl_Position;
        EmitVertex();
    }
   
    EndPrimitive();
}    