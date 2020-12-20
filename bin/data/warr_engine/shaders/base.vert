#version 420 core

layout (location = 0) in vec3 vs_pos;
layout (location = 1) in vec2 vs_tex_coord;
layout (location = 2) in vec4 vs_color;
layout (location = 3) in float vs_emissive;

// ----------------------------------------------------------------------------

uniform mat4 u_projection_matrix;
uniform mat4 u_view_matrix;            // camera

// ----------------------------------------------------------------------------

out vec3 fs_pos;
out vec2 fs_tex_coord;
out vec4 fs_color;
out float fs_emissive;

// ----------------------------------------------------------------------------

void main()
{
    fs_pos = vs_pos;

    // apply matrix transforms to get the vertex to screen space.
	// "world space to camera space" * "camera space to projection space" * VertexPosition

    gl_Position = u_projection_matrix * u_view_matrix * vec4( fs_pos, 1.0 );

    // pass values to fragment shader

    fs_tex_coord = vs_tex_coord;
    fs_color = vs_color;
    fs_emissive = vs_emissive;
}
