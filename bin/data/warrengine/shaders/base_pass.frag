#version 330 core

// ----------------------------------------------------------------------------
// base pass
// ----------------------------------------------------------------------------
//
// this is what draws the index/vertex buffers within each draw call.
//
// each pixel drawn goes into 3 color attachments:
//
// color : the base output texture
// glow  : any bright pixels that need to be blurred and overlaid later
// pick  : entity ID values for mouse picking
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

uniform float u_viewport_w = 320.0;
uniform float u_viewport_h = 240.0;

#pragma include "get_sample_from_texture.frag"

uniform float glow_minimum = 0.1f;

void main()
{
	// ----------------------------------------------------------------------------
	// "color"

	out_color_buffer = get_sample_from_texture( fs_texture_id, fs_tex_coord ) * fs_color;

	// ----------------------------------------------------------------------------
	// "glow"

	out_glow_buffer = vec4( 0.0f, 0.0f, 0.0f, 1.0f );


	if( fs_glow > 0.0f )
	{
		vec3 glow_color = out_color_buffer.rgb * fs_glow;

		float avg = (glow_color.r + glow_color.g + glow_color.b) / 3.0f;
		if( avg > glow_minimum )
		{
			out_glow_buffer = out_color_buffer * fs_glow;
			out_glow_buffer.a = out_color_buffer.a;
		}
	}

	// ----------------------------------------------------------------------------
	// "pick"

	if( fs_pick_id > 0.0f )
	{
		out_pick_ids_buffer = vec4( fs_pick_id, 0, 0, 1.0f );
	}
}
