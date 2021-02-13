#version 330 core
/*
    Input: [vec2] texture coords from vertex shader
    Output: [vec4] color of text render by textColor
*/

in vec3 vNormal;
in vec3 vPos;

out vec4 color;

uniform vec3 uColor;
uniform vec3 uLightDir;
uniform vec3 uEyePos;
uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;

void main(){
    vec3 N = normalize(vNormal);
    vec3 R = normalize( reflect(-uLightDir, N) );


    vec3 V = normalize(uEyePos - vPos);
    float diffuse = max(dot(uLightDir, N), 0);

    float specular = pow( max( dot(R, V), 0 ), 128 );

    vec3 finalColor = ambientColor + diffuseColor * diffuse + specularColor * specular;
    color = vec4(finalColor, 1);
}