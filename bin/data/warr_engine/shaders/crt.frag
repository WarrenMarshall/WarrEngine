#version 420 core
out vec4 FragColor;

in vec2 TexCoord;
in vec4 Color;
in vec3 Pos;

in flat float _current_time;

in flat int _show_vignette;
in flat float _var_vignette_size;
in flat float _var_vignette_smoothness;
in flat float _var_vignette_rounding;

in flat int _show_crt_tint;

in flat int _show_crt_warp;
in flat float _var_crt_warp_bend;

in flat int _show_crt_scanlines;
in flat float _var_crt_scanlines_intensity;

in flat int _show_chromatic_abberation;
in flat float _var_chromatic_abberation_amount;

in flat int _show_desaturation;
in flat float _var_desaturation_amount;

in flat int _show_inverted;

in flat vec4 _var_color_tint;

in flat vec4 _var_color_overlay;

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
	return sin( uv.y * lines + _current_time * speed );
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

	if( _show_crt_warp > 0 )
	{
		vec2 crt_uv = fx_crt_warp( final_uv, _var_crt_warp_bend );

		// adjust final_color to be fully black if outside the 0-1 range in UV coords

		//if( crt_uv.x <= 0.0f || crt_uv.x >= 1.0f ) final_color = vec4(0,0,0,1);
		//if( crt_uv.y <= 0.0f || crt_uv.y >= 1.0f ) final_color = vec4(0,0,0,1);

		final_uv = crt_uv;
	}

	// ----------------------------------------------------------------------------
	// rolling scan lines
	// ----------------------------------------------------------------------------

	if( _show_crt_scanlines > 0 )
	{
		float s1 = fx_scanline( final_uv, 20.0f, -10.0f );
		float s2 = fx_scanline( final_uv, 2.0f, -3.0f );

		final_color = mix( final_color, vec4( s1 + s2 ), _var_crt_scanlines_intensity );
	}

	// ----------------------------------------------------------------------------
	// tint every other line to create cheap CRT effect
	// ----------------------------------------------------------------------------

	float crt_tint = 1.0f;
	float crt_tint_inv = 0.0f;

	if( _show_crt_tint > 0 )
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

	if( _show_chromatic_abberation > 0 )
	{
		FragColor = texture( ourTexture, final_uv );
		FragColor.r = texture(ourTexture, vec2(final_uv.x - _var_chromatic_abberation_amount, final_uv.y - _var_chromatic_abberation_amount)).r;
		FragColor.b = texture(ourTexture, vec2(final_uv.x + _var_chromatic_abberation_amount, final_uv.y + _var_chromatic_abberation_amount)).b;
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

	if( _show_desaturation > 0 )
	{
		vec3 rgb = vec3( FragColor.r, FragColor.g, FragColor.b );
		float T = _var_desaturation_amount;
		FragColor = vec4( mix( vec3( dot( rgb, vec3( 0.2125, 0.7154, 0.0721 ) ) ), rgb, T ), FragColor.a );
	}

	// ----------------------------------------------------------------------------
	// invert
	//
	// does a "1.0 - color" fragment color replacement.
	// ----------------------------------------------------------------------------

	if( _show_inverted > 0 )
	{
		FragColor = vec4( 1.0 - FragColor.r, 1.0 - FragColor.g, 1.0 - FragColor.b, FragColor.a );
	}

	// ----------------------------------------------------------------------------
	// color tint
	//
	// gets added to the fragment color. good for damage indicators.
	// ----------------------------------------------------------------------------

	vec3 color_tint = vec3(
		_var_color_tint.r * _var_color_tint.a,
		_var_color_tint.g * _var_color_tint.a,
		_var_color_tint.b * _var_color_tint.a
	);

	FragColor += vec4( color_tint, 1.0f );

	// ----------------------------------------------------------------------------
	// color overlay
	//
	// replaces the fragment color in a lerp.  good for color fades, like to black.
	// ----------------------------------------------------------------------------

	FragColor = mix(
		FragColor,
		vec4(_var_color_overlay.r, _var_color_overlay.g, _var_color_overlay.b, 1.0f),
		_var_color_overlay.a
	);

	// ----------------------------------------------------------------------------
	// vignette
	// ----------------------------------------------------------------------------

	if( _show_vignette > 0 )
	{
	  	FragColor *= fx_vignette( final_uv, _var_vignette_size, _var_vignette_smoothness, _var_vignette_rounding );
	}
}
