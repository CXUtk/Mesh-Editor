#version 330 core
/*
    Input: [vec2] texture coords from vertex shader
    Output: [vec4] color of text render by textColor
*/
out vec4 color;
uniform vec4 uColor;

void main(){
    color = uColor;
}