#version 300 es

layout (location = 0) in vec3 aCoords;

out vec3 vCoords;

void main()
{
    vCoords = aCoords;
    gl_Position = vec4(aCoords, 1.0);
}
