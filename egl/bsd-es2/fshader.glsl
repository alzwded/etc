#version 100
precision highp float;

//layout(location = 0) out vec4 fragColor;

//in vec3 vCoords;
varying vec3 vCoords;

void main()
{
    gl_FragColor = vec4(vCoords.xy/2.0 + 0.5, 0.0, 1.0);
    //fragColor = mix(vec4(1, 0, 0, 1), vec4(1, 1, 0, 1), vCoords.y/2.0 + 0.5);
}
