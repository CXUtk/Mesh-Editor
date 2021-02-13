#version 150

// Flat shading vertex shader
//
// Does all vertex transformation and color calculation,
// except doesn't create specular highlights.

// INCOMING DATA

// Vertex location (in model space)
in vec4 vPosition;

// Normal vector at vertex (in model space)
in vec3 vNormal;

//
// Uniform data
//

// Transformations
uniform mat4 vMat;  // view (camera)
uniform mat4 pMat;  // projection

uniform mat4 tMat;  // translation
uniform mat4 sMat;  // scaling
uniform mat4 xMat;  // x rotation
uniform mat4 yMat;  // y rotation
uniform mat4 zMat;  // z rotation

// Light position is given in world space
uniform vec4 lightPosition;

// Light color
uniform vec4 lightColor;
uniform vec4 ambientLight;

// Material properties
uniform vec4 diffuseColor;
uniform vec4 ambientColor;
uniform vec3 kCoeff;

// OUTGOING DATA

// Color to "attach" to vertex and get sent to fragment shader
out vec4 color;

void main()
{
    // create the model and modelview matrices
    // transformation order: scale, rotate Z, rotate Y, rotate X, translate
    mat4 modelMat = tMat * xMat * yMat * zMat * sMat;
    mat4 modelViewMat = vMat * modelMat;

    // All vectors need to be converted to "eye" space
    // All vectors should also be normalized
    vec4 vertexInEye = modelViewMat * vPosition;
    vec4 lightInEye = vMat * lightPosition;

    // "Correct" way to transform normals.  The normal matrix is the
    // inverse transpose of the upper left 3x3 submatrix of the modelView
    // matrix (i.e., does not include translations).  THIS IS EXPENSIVE
    // TO COMPUTE because of the inverse(), and should really be done in
    // the application, not here.
    mat3 normMat = inverse( transpose( mat3(modelViewMat) ) );
    vec4 normalInEye = vec4( normMat * vNormal, 0.0 );

    // calculate relevant lighting vectors
    vec4 L = normalize (lightInEye - vertexInEye);
    vec4 N = normalize (normalInEye);

    // calculate components
    vec4 ambient = ambientLight * ambientColor;
    vec4 diffuse = lightColor * diffuseColor * max(dot(N,L),0.0);

    // calculate the final color
    color = (kCoeff.x * ambient) +
            (kCoeff.y * diffuse);

    // transform the vertex location into clip space
    gl_Position =  pMat * modelViewMat * vPosition;
}
