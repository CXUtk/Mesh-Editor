#version 330 core
/*
    Input: [vec2] texture coords from vertex shader
    Output: [vec4] color of text render by textColor
*/

in vec2 texCoords;
out vec4 color;

uniform vec3 uColor;
uniform float uThickness;

void main(){
    vec2 vDis = texCoords - vec2(0.5, 0.5);
    float dis = length(vDis);
    if (dis < 0.5 && dis > 0.5 - uThickness) {
        color = vec4(uColor, 1);
        return;
    }
    color = vec4(0, 0, 0, 0);
}