#version 330 core
/*
    Input: [vec2] texture coords from vertex shader
    Output: [vec4] color of text render by textColor
*/

in vec2 texCoords;
out vec4 color;

uniform sampler2D uTexure;
uniform vec3 uColor;

void main(){
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(uTexure, texCoords).r);
    color = vec4(uColor, 1.0) * sampled;
}