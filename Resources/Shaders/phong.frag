#version 150

// Phong shading fragment shader
//
// Uses shading information provided by the vertex shader
// to do Phong shading calculations.

// INCOMING DATA

// Light color
uniform vec4 lightColor;
uniform vec4 ambientLight;

// Material properties
uniform vec4 diffuseColor;
uniform vec4 ambientColor;
uniform vec4 specularColor;
uniform float specExp;
uniform vec3 kCoeff;

// Vectors "attached" to vertex by vertex shader and interpolated

// Light position
in vec3 lPos;

// Vertex position (in clip space)
in vec3 vPos;

// Vertex normal
in vec3 vNorm;

// OUTGOING DATA
out vec4 fragColor;

void main()
{
    // calculate lighting vectors
    vec3 L = normalize( lPos - vPos );
    vec3 N = normalize( vNorm );
    vec3 R = normalize( reflect(-L, N) );
    vec3 V = normalize( -(vPos) );

    // calculate components
    vec4 ambient = ambientColor * ambientLight;
    vec4 diffuse = lightColor * diffuseColor * max(dot(N,L),0.0);
    float specDot = pow( max(dot(R,V),0.0), specExp );
    vec4 specular = lightColor * specularColor * specDot;

    // calculate the final color
    vec4 color = (kCoeff.x * ambient) +
                 (kCoeff.y * diffuse) +
                 (kCoeff.z * specular);

    fragColor = color;
}
