#version 330 core
/**
* Default Shader
*/
layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;

uniform mat4 projection;
uniform mat4 model;

out vec3 vNormal;
out vec3 vPos;

void main(){
    vPos = vertex;
    
    vec4 pos = projection * model * vec4(vertex, 1.0);
    gl_Position = pos;

    mat4 normalTrans = transpose(inverse(model));
    vNormal = vec3(model * vec4(normal, 0.0));
}

