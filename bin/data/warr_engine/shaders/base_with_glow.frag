#version 420 core

layout (location = 0) out vec4 out_color_buffer;
layout (location = 1) out vec4 out_glow_buffer;

in vec3 fs_pos;
in vec2 fs_tex_coord;
in vec4 fs_color;
in float fs_glow;
in float fs_texture_id;

// set to "true" to have the screen automatically glow if
// pixel colors are bright enough.

uniform bool ub_auto_glow = false;
uniform float u_auto_glow_lower_limit = 1.0f;

uniform sampler2D u_textures[__max_texture_image_units__];

void main()
{
	int texture_idx = int(fs_texture_id);

	// ------------------------------------
	// first color buffer
	//
	// - regular rendering

	out_color_buffer = texture( u_textures[texture_idx], fs_tex_coord ) * fs_color;

	// ------------------------------------
	// second color buffer - "glow"
	//

	// if "auto glow" is enabled, the entire frame buffer has a chance to glow. pixels
	// will be added to the glow buffer if they exceed "u_auto_glow_lower_limit" in
	// brightness.

	out_glow_buffer = vec4( 0.0f, 0.0f, 0.0f, 1.0f );

	if( ub_auto_glow )
	{
		float brightness = dot( out_color_buffer.rgb, vec3( 0.2126f, 0.7152f, 0.0722f ) );

		if( brightness >= u_auto_glow_lower_limit )
		{
			out_glow_buffer = out_color_buffer;
		}
	}

	// "fs_glow" is used to explicitly set pixels to glow and is passed to
	// the shaders in the vertex data stream.
	//
	// value ranges from 0.0f-N.0f

	if( fs_glow > 0.0f )
	{
		vec3 glow_color = vec3( out_color_buffer.r, out_color_buffer.g, out_color_buffer.b ) * fs_glow;

		out_glow_buffer = vec4(
			max( out_glow_buffer.r, glow_color.r ),
			max( out_glow_buffer.g, glow_color.g ),
			max( out_glow_buffer.b, glow_color.b ),
			out_color_buffer.a
		);
	}
}
