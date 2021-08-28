#version 330 core

// ----------------------------------------------------------------------------
// final_pass
// ----------------------------------------------------------------------------
//
// copies the composite color attachment into the default opengl frame buffer.
//
// this is where any effects are applied that need to operate in screen space.
// these can be expensive since you're touching every pixel that is being
// drawn on the monitor, so be thoughtful when adding new effects.
//
// ----------------------------------------------------------------------------

layout (location = 0) out vec4 out_color_buffer;

in vec3 fs_pos;
in vec2 fs_tex_coord;
in vec4 fs_color;
in float fs_glow;
flat in int fs_texture_id;
flat in int fs_pick_id;

uniform float u_current_time;		// updated by the engine each frame
uniform float u_viewport_w = 320.0;
uniform float u_viewport_h = 240.0;

#pragma include "get_sample_from_texture.frag"

// ----------------------------------------------------------------------------
// > crt warp - warp UVs so the screen appears curved like an old crt screen
// ----------------------------------------------------------------------------

uniform bool ub_crt_warp = false;
uniform float u_crt_warp_bend = 4.0f;

vec2 fx_crt_warp_wk( vec2 uv, float bend )
{
	uv -= 0.5f;
	uv *= 2.0f;

	uv.x *= 1.0f + pow( abs(uv.y) / bend, 2.0f );
	uv.y *= 1.0f + pow( abs(uv.x) / bend, 2.0f );

	uv /= 2.0f;
	return uv + 0.5f;
}

vec2 fx_crt_warp( vec2 output_uvs )
{
	if( ub_crt_warp )
	{
		output_uvs = fx_crt_warp_wk( output_uvs, u_crt_warp_bend );
	}

	return output_uvs;
}

// ----------------------------------------------------------------------------
// > pixelate - snaps the UV coordinates to a specific step for chunky pixels
// ----------------------------------------------------------------------------

uniform bool ub_pixelate = false;
uniform float u_pixelate_factor = 2.0f;

vec2 fx_pixelate( vec2 output_uvs )
{
	if( ub_pixelate && u_pixelate_factor > 1.0 )
	{
		vec2 pixel_density = vec2( u_viewport_w / u_pixelate_factor, u_viewport_h / u_pixelate_factor );
		vec2 aspect_ratio_multiplier;

		if( u_viewport_h > u_viewport_w )
		{
			aspect_ratio_multiplier = vec2( u_viewport_w / u_viewport_h, 1.0 );
		}
		else
		{
			aspect_ratio_multiplier = vec2( 1.0, u_viewport_h / u_viewport_w );
		}

		vec2 pixelScaling = pixel_density * aspect_ratio_multiplier;
		output_uvs = round( output_uvs * pixelScaling ) / pixelScaling;
	}

	return output_uvs;
}

// ----------------------------------------------------------------------------

void main()
{
	// ----------------------------------------------------------------------------
	// default handling of fragments
	// ----------------------------------------------------------------------------

	vec2 uv_coord = fs_tex_coord;

	// post process stack (for uv coordinates)

	uv_coord = fx_pixelate( uv_coord );
	uv_coord = fx_crt_warp( uv_coord );

	// adjust final_color to be fully black if outside the 0-1 range in UV coords
	if( uv_coord.x < 0.0f || uv_coord.x > 1.0f || uv_coord.y < 0.0f || uv_coord.y > 1.0f )
	{
		out_color_buffer = vec4(0,0,0,1);
	}
	else
	{
		out_color_buffer = get_sample_from_texture( fs_texture_id, uv_coord );
	}

	/*
	// note : it is assumed that texture 1 is the LUT texture
	//
	// using the r,g,b values from the color buffer, look up the final color
	// in the LUT texture.

	float r = clamp( out_color_buffer.r, 0.f, 1.f );
	float g = clamp( out_color_buffer.g, 0.f, 1.f );
	float b = clamp( out_color_buffer.b, 0.f, 1.f );

	vec3 lut = vec3(
		texture( u_textures[ 1 ], vec2( out_color_buffer.r, 0.875 ) ).r,
		texture( u_textures[ 1 ], vec2( out_color_buffer.g, 0.625 ) ).g,
		texture( u_textures[ 1 ], vec2( out_color_buffer.b, 0.375 ) ).b
	);

	out_color_buffer.rgb = lut;
	*/
}
