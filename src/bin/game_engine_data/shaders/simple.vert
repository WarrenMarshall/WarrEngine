#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec4 aColor;

uniform mat4 P; // projection matrix
uniform mat4 V; // camera matrix

out vec2 TexCoord;
out vec4 Color;

void main()
{
	// "world space to camera space" * "camera space to projection space"
    gl_Position = P * V * vec4( aPos.x, aPos.y, aPos.z, 1.0 );

    TexCoord = aTexCoord;
    Color = aColor;
}
