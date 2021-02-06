#version 330 core
/*
    Input: [vec2] texture coords from vertex shader
    Output: [vec4] color of text render by textColor
*/
out vec4 color;
uniform vec3 uColor;

void main(){
    color = vec4(uColor, 1);
}