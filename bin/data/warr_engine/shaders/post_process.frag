#version 420 core
out vec4 FragColor;

in vec3 Pos;
in vec2 TexCoord;
in vec4 Color;
in float Emissive;

uniform float u_current_time;

uniform bool b_vignette = false;
uniform float vignette_size = 1.9f;
uniform float vignette_smoothness = 0.75f;
uniform float vignette_rounding = 16.0f;

uniform bool b_crt_tint = false;

uniform bool b_crt_warp = false;
uniform float crt_warp_bend = 4.0f;

uniform bool b_crt_scanlines = false;
uniform float crt_scanlines_intensity = 0.01f;

uniform bool b_chromatic_abberation = false;
uniform float chromatic_abberation_amount = 0.0025f;

uniform bool b_desaturation = false;
uniform float desaturation_amount = 0.15f;

uniform bool b_inverted = false;

uniform vec4 color_tint = vec4(  1.0f, 1.0f, 1.0f, 0.0f );

uniform vec4 color_overlay = vec4(  0.0f, 0.0f, 0.0f, 0.0f );

uniform sampler2D ourTexture;

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

	vec4 final_color = Color;
	vec2 final_uv = TexCoord;

	// ----------------------------------------------------------------------------
	// CRT bending of image in the corners
	// ----------------------------------------------------------------------------

	if( b_crt_warp )
	{
		vec2 crt_uv = fx_crt_warp( final_uv, crt_warp_bend );

		// adjust final_color to be fully black if outside the 0-1 range in UV coords

		//if( crt_uv.x <= 0.0f || crt_uv.x >= 1.0f ) final_color = vec4(0,0,0,1);
		//if( crt_uv.y <= 0.0f || crt_uv.y >= 1.0f ) final_color = vec4(0,0,0,1);

		final_uv = crt_uv;
	}

	// ----------------------------------------------------------------------------
	// rolling scan lines
	// ----------------------------------------------------------------------------

	if( b_crt_scanlines )
	{
		float s1 = fx_scanline( final_uv, 20.0f, -10.0f );
		float s2 = fx_scanline( final_uv, 2.0f, -3.0f );

		final_color = mix( final_color, vec4( s1 + s2 ), crt_scanlines_intensity );
	}

	// ----------------------------------------------------------------------------
	// tint every other line to create cheap CRT effect
	// ----------------------------------------------------------------------------

	float crt_tint = 1.0f;
	float crt_tint_inv = 0.0f;

	if( b_crt_tint )
	{
		if( (int( Pos.y ) % 2) > 0 )
		{
			crt_tint = 0.975f;
			crt_tint_inv = 0.01f;
		}

	   	final_color *= crt_tint;
	}

	// ----------------------------------------------------------------------------
	// chromatic abberation
	// ----------------------------------------------------------------------------

	if( b_chromatic_abberation )
	{
		FragColor = texture( ourTexture, final_uv );
		FragColor.r = texture(ourTexture, vec2(final_uv.x - chromatic_abberation_amount, final_uv.y - chromatic_abberation_amount)).r;
		FragColor.b = texture(ourTexture, vec2(final_uv.x + chromatic_abberation_amount, final_uv.y + chromatic_abberation_amount)).b;
	}
	else
	{
		FragColor = texture( ourTexture, final_uv );
	}

	// ----------------------------------------------------------------------------

	FragColor *= final_color;

	// #todo - all of these color effects change what they do based on the order they are
	// applied. it would be nice to be able to customize this somehow

	// ----------------------------------------------------------------------------
	// desaturate
	// ----------------------------------------------------------------------------

	if( b_desaturation )
	{
		vec3 rgb = vec3( FragColor.r, FragColor.g, FragColor.b );
		float T = desaturation_amount;

		FragColor = vec4( mix( vec3( dot( rgb, vec3( 0.2125, 0.7154, 0.0721 ) ) ), rgb, T ), FragColor.a );
	}

	// ----------------------------------------------------------------------------
	// invert
	//
	// does a "1.0 - color" fragment color replacement.
	// ----------------------------------------------------------------------------

	if( b_inverted )
	{
		FragColor = vec4( 1.0 - FragColor.r, 1.0 - FragColor.g, 1.0 - FragColor.b, FragColor.a );
	}

	// ----------------------------------------------------------------------------
	// color tint
	//
	// gets added to the fragment color. good for damage indicators.
	// ----------------------------------------------------------------------------

	vec3 color_tint = vec3(
		color_tint.r * color_tint.a,
		color_tint.g * color_tint.a,
		color_tint.b * color_tint.a
	);

	FragColor += vec4( color_tint, 1.0f );

	// ----------------------------------------------------------------------------
	// color overlay
	//
	// replaces the fragment color in a lerp.  good for color fades, like to black.
	// ----------------------------------------------------------------------------

	FragColor = mix(
		FragColor,
		vec4(color_overlay.r, color_overlay.g, color_overlay.b, 1.0f),
		color_overlay.a
	);

	// ----------------------------------------------------------------------------
	// vignette
	// ----------------------------------------------------------------------------

	if( b_vignette )
	{
	  	FragColor *= fx_vignette( final_uv, vignette_size, vignette_smoothness, vignette_rounding );
	}

	//BrightColor = vec4( 1.0f, 0.0f, 0.0f, 1.0f );
}
