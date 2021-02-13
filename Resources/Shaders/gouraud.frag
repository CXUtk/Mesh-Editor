#version 150

// Gouraud shading fragment shader
//
// Just applies color computed by the vertex shader.

// INCOMING DATA

// Color "attached" to fragment
in vec4 color;

// OUTGOING DATA
out vec4 fragColor;

void main()
{
    fragColor = color;
}
