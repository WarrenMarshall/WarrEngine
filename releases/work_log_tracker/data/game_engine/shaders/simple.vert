#version 330 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inTexCoord;
layout (location = 2) in vec4 inColor;

uniform mat4 P; // projection matrix
uniform mat4 V; // camera matrix
uniform float in_time;
uniform float in_use_vignette;

out vec2 TexCoord;
out vec4 Color;
out vec3 Pos;
out float _time;
out float _use_vignette;

void main()
{
	// snap the vertex to the pixel grid. this gives the game
	// a sharp pixel perfect look.

    vec3 snapped_pos;

    snapped_pos.x = floor( inPos.x );
    snapped_pos.y = floor( inPos.y );
    snapped_pos.z = floor( inPos.z );

    Pos = snapped_pos;

    // apply matrix transforms to get the vertex to screen space.
    //
	// "world space to camera space" * "camera space to projection space"

    gl_Position = P * V * vec4( snapped_pos.x, snapped_pos.y, snapped_pos.z, 1.0 );

    TexCoord = inTexCoord;
    Color = inColor;
    _time = in_time;
    _use_vignette = in_use_vignette;
}
