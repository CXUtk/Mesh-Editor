#version 330 core
/**
* Default Shader
*/
layout (location = 0) in vec3 vertex;

uniform mat4 projection;
uniform mat4 model;

void main(){
    gl_Position = projection * model * vec4(vertex, 1);
}

