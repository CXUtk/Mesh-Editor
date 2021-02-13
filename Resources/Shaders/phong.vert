#version 150

// Phong shading vertex shader
//
// Does all vertex transformation, and set up relevant
// shading vectors to be passed to the fragment shader.

// INCOMING DATA

// Vertex position (in model space)
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

// OUTGOING DATA

// Vectors to "attach" to vertex and get sent to fragment shader
// Vectors and points will be passed in "eye" space
out vec3 lPos;
out vec3 vPos;
out vec3 vNorm;

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

    // pass our vertex data to the fragment shader
    lPos = lightInEye.xyz;
    vPos = vertexInEye.xyz;
    vNorm = normalInEye.xyz;

    // send the vertex position into clip space
    gl_Position =  pMat * modelViewMat * vPosition;
}
