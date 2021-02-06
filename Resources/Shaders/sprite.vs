#version 330 core
/**
* Default Shader
*/
layout (location = 0) in vec4 vData;

out vec2 texCoords;
uniform mat4 projection;
uniform mat4 model;

void main(){
    gl_Position = projection * model * vec4(vData.xy, 0, 1);
    texCoords = vData.zw;
}

