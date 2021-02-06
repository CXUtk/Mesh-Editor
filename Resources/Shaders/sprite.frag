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
    color = vec4(uColor, 1) * texture(uTexure, texCoords);
}