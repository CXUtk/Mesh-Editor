#version 330 core
/*
    Input: [vec2] texture coords from vertex shader
    Output: [vec4] color of text render by textColor
*/

in vec3 vNormal;

out vec4 color;

uniform vec3 uColor;
uniform vec3 uLightDir;

void main(){
    vec3 N = normalize(vNormal);
    float v = max(dot(uLightDir, N), 0) * 0.8 + 0.2;
    color = vec4(uColor * v, 1);
}