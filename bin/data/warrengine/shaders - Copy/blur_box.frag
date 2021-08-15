#version 330 core

// ----------------------------------------------------------------------------
// blur_box
// ----------------------------------------------------------------------------
//
// copies one color attachment into another, applying a box filter algorithm
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

uniform float u_kernel_size = 15;
uniform float u_viewport_w = 320.0;
uniform float u_viewport_h = 240.0;

// ----------------------------------------------------------------------------

void main()
{
	vec2 texel_sizes = vec2( 1.0 / u_viewport_w, 1.0 / u_viewport_h );

	vec3 avg_color = vec3( 0.0, 0.0, 0.0 );

	float upper = ( ( u_kernel_size - 1 ) / 2 );
	float lower = -upper;

	for( float x = lower ; x <= upper ; ++x )
	{
		for( float y = lower ; y <= upper ; ++y )
		{
			vec2 offset = vec2( texel_sizes.x * x, texel_sizes.y * y );
			avg_color += texture( u_textures[0], fs_tex_coord + offset ).rgb;
		}
	}

	avg_color /= ( u_kernel_size * u_kernel_size );

	out_color_buffer = vec4( avg_color, 1.0 );
}
