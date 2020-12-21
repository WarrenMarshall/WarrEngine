#version 420 core
out vec4 out_fragment_color;

in vec3 fs_pos;
in vec2 fs_tex_coord;
in vec4 fs_color;
in float fs_emissive;
in float fs_texture_id;

uniform float u_current_time;

uniform bool ub_vignette = false;
uniform float u_vignette_size = 1.9f;
uniform float u_vignette_smoothness = 0.75f;
uniform float u_vignette_rounding = 16.0f;

uniform bool ub_crt_tint = false;

uniform bool ub_crt_warp = false;
uniform float u_crt_warp_bend = 4.0f;

uniform bool ub_crt_scanlines = false;
uniform float u_crt_scanlines_intensity = 0.01f;

uniform bool ub_chromatic_abberation = false;
uniform float u_chromatic_abberation_amount = 0.0025f;

uniform bool ub_desaturation = false;
uniform float u_desaturation_amount = 0.15f;

uniform bool ub_inverted = false;

uniform vec4 u_color_tint = vec4(  1.0f, 1.0f, 1.0f, 0.0f );

uniform vec4 u_color_overlay = vec4(  0.0f, 0.0f, 0.0f, 0.0f );

uniform sampler2D u_texture_0;

// ----------------------------------------------------------------------------
// warp UVs so the screen appears curved like an old crt screen.

vec2 fx_crt_warp( vec2 uv, float bend )
{
	uv -= 0.5f;
	uv *= 2.0f;

	uv.x *= 1.0f + pow( abs(uv.y) / bend, 2.0f );
	uv.y *= 1.0f + pow( abs(uv.x) / bend, 2.0f );

	uv /= 2.0f;
	return uv + 0.5f;
}

// ----------------------------------------------------------------------------
// darkened edge around the outside

float fx_vignette( vec2 uv, float size, float smoothness, float edgeRounding )
{
	uv -= 0.5f;
	uv *= size;

	float amount = sqrt( pow( abs( uv.x ), edgeRounding ) + pow( abs( uv.y ), edgeRounding ) );

	amount = 1.0f - amount;

	return smoothstep( 0.0f, smoothness, amount );
}

// ----------------------------------------------------------------------------
// horizontal lines that run horizontally across the screen and move over time

float fx_scanline( vec2 uv, float lines, float speed )
{
	return sin( uv.y * lines + u_current_time * speed );
}

void main()
{
	// ----------------------------------------------------------------------------
	// default handling of fragments
	// ----------------------------------------------------------------------------

	vec4 final_color = fs_color;
	vec2 final_uv = fs_tex_coord;

	// ----------------------------------------------------------------------------
	// CRT bending of image in the corners
	// ----------------------------------------------------------------------------

	if( ub_crt_warp )
	{
		vec2 crt_uv = fx_crt_warp( final_uv, u_crt_warp_bend );

		// adjust final_color to be fully black if outside the 0-1 range in UV coords

		//if( crt_uv.x <= 0.0f || crt_uv.x >= 1.0f ) final_color = vec4(0,0,0,1);
		//if( crt_uv.y <= 0.0f || crt_uv.y >= 1.0f ) final_color = vec4(0,0,0,1);

		final_uv = crt_uv;
	}

	// ----------------------------------------------------------------------------
	// rolling scan lines
	// ----------------------------------------------------------------------------

	if( ub_crt_scanlines )
	{
		float s1 = fx_scanline( final_uv, 20.0f, -10.0f );
		float s2 = fx_scanline( final_uv, 2.0f, -3.0f );

		final_color = mix( final_color, vec4( s1 + s2 ), u_crt_scanlines_intensity );
	}

	// ----------------------------------------------------------------------------
	// tint every other line to create cheap CRT effect
	// ----------------------------------------------------------------------------

	float crt_tint = 1.0f;
	float crt_tint_inv = 0.0f;

	if( ub_crt_tint )
	{
		if( (int( fs_pos.y ) % 2) > 0 )
		{
			crt_tint = 0.975f;
			crt_tint_inv = 0.01f;
		}

	   	final_color *= crt_tint;
	}

	// ----------------------------------------------------------------------------
	// chromatic abberation
	// ----------------------------------------------------------------------------

	if( ub_chromatic_abberation )
	{
		out_fragment_color = texture( u_texture_0, final_uv );
		out_fragment_color.r = texture(u_texture_0, vec2(final_uv.x - u_chromatic_abberation_amount, final_uv.y - u_chromatic_abberation_amount)).r;
		out_fragment_color.b = texture(u_texture_0, vec2(final_uv.x + u_chromatic_abberation_amount, final_uv.y + u_chromatic_abberation_amount)).b;
	}
	else
	{
		out_fragment_color = texture( u_texture_0, final_uv );
	}

	// ----------------------------------------------------------------------------

	out_fragment_color *= final_color;

	// #todo - all of these color effects change what they do based on the order they are
	// applied. it would be nice to be able to customize this somehow

	// ----------------------------------------------------------------------------
	// desaturate
	// ----------------------------------------------------------------------------

	if( ub_desaturation )
	{
		vec3 rgb = vec3( out_fragment_color.r, out_fragment_color.g, out_fragment_color.b );
		float T = u_desaturation_amount;

		out_fragment_color = vec4( mix( vec3( dot( rgb, vec3( 0.2125, 0.7154, 0.0721 ) ) ), rgb, T ), out_fragment_color.a );
	}

	// ----------------------------------------------------------------------------
	// invert
	//
	// does a "1.0 - color" fragment color replacement.
	// ----------------------------------------------------------------------------

	if( ub_inverted )
	{
		out_fragment_color = vec4( 1.0 - out_fragment_color.r, 1.0 - out_fragment_color.g, 1.0 - out_fragment_color.b, out_fragment_color.a );
	}

	// ----------------------------------------------------------------------------
	// color tint
	//
	// gets added to the fragment color. good for damage indicators.
	// ----------------------------------------------------------------------------

	vec3 color_tint = vec3(
		u_color_tint.r * u_color_tint.a,
		u_color_tint.g * u_color_tint.a,
		u_color_tint.b * u_color_tint.a
	);

	out_fragment_color += vec4( color_tint, 1.0f );

	// ----------------------------------------------------------------------------
	// color overlay
	//
	// replaces the fragment color in a lerp.  good for color fades, like to black.
	// ----------------------------------------------------------------------------

	out_fragment_color = mix(
		out_fragment_color,
		vec4(u_color_overlay.r, u_color_overlay.g, u_color_overlay.b, 1.0f),
		u_color_overlay.a
	);

	// ----------------------------------------------------------------------------
	// vignette
	// ----------------------------------------------------------------------------

	if( ub_vignette )
	{
	  	out_fragment_color *= fx_vignette( final_uv, u_vignette_size, u_vignette_smoothness, u_vignette_rounding );
	}
}
