#version 330 core

// ----------------------------------------------------------------------------
// simple
// ----------------------------------------------------------------------------
//
// a pass-thru fragment shader. doesn't do anything other than sample
// a texture and output the color.
//
// ----------------------------------------------------------------------------

layout (location = 0) out vec4 out_color_buffer;

in vec3 fs_pos;
in vec2 fs_tex_coord;
in vec4 fs_color;
in float fs_glow;
flat in int fs_texture_id;
flat in int fs_pick_id;

#pragma include "get_sample_from_texture.frag"

void main()
{
	out_color_buffer = get_sample_from_texture( fs_texture_id, fs_tex_coord ) * fs_color;
}
