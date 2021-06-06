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
layout (location = 1) out vec4 out_glow_buffer;
layout (location = 2) out vec4 out_pick_ids_buffer;
layout (location = 3) out vec4 out_blur_buffer;
layout (location = 4) out vec4 out_composite_buffer;
layout (location = 5) out vec4 out_final_buffer;

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
