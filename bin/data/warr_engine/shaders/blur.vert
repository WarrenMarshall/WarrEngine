#version 420 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inTexCoord;
layout (location = 2) in vec4 inColor;

// ----------------------------------------------------------------------------

// projection matrix
uniform mat4 P;

// camera matrix
uniform mat4 V;

// ----------------------------------------------------------------------------

out vec2 TexCoord;
out vec4 Color;
out vec3 Pos;

void main()
{
    Pos = inPos;

    // apply matrix transforms to get the vertex to screen space.
	// "world space to camera space" * "camera space to projection space" * VertexPosition

    gl_Position = P * V * vec4( Pos.x, Pos.y, Pos.z, 1.0 );

    // pass values to fragment shader

    TexCoord = inTexCoord;
    Color = inColor;
}
