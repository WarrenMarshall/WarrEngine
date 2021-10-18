#version 330 core

layout (location = 0) in vec3 vs_pos;
layout (location = 1) in vec2 vs_tex_coord;
layout (location = 2) in vec4 vs_color;
layout (location = 3) in float vs_glow;
layout (location = 4) in int vs_texture_id;
layout (location = 5) in int vs_pick_id;

// ----------------------------------------------------------------------------

uniform mat4 u_projection_matrix;		// view space to clip space
uniform mat4 u_view_matrix;				// world space to view space
uniform mat4 u_viewport_pivot_matrix;	// a viewport offset so you can move where the world origin is (say, middle of the screen)

// ----------------------------------------------------------------------------

out vec3 fs_pos;					// vertex position in world space
out vec2 fs_tex_coord;
out vec4 fs_color;
out float fs_glow;
flat out int fs_texture_id;
flat out int fs_pick_id;

// ----------------------------------------------------------------------------

void main()
{
	fs_pos = vs_pos;

	// snap verts to pixel positions
	//
	// note : without this you will have problems with things drawing at
	// sizes that are slightly off and text not aligning and such. leave this
	// enabled or face the demons.
	//
	// #option - maybe this should be an option thae games can set in thier INI files

	fs_pos.x = floor( fs_pos.x + 0.5 );
	fs_pos.y = floor( fs_pos.y + 0.5 );

	gl_Position = u_projection_matrix * u_viewport_pivot_matrix * u_view_matrix * vec4( fs_pos, 1.0 );

	// pass values to fragment shader

	fs_tex_coord = vs_tex_coord;
	fs_color = vs_color;
	fs_glow = vs_glow;
	fs_texture_id = vs_texture_id;
	fs_pick_id = vs_pick_id;
}
