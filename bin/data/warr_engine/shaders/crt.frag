#version 420 core
out vec4 FragColor;

in vec2 TexCoord;
in vec4 Color;
in vec3 Pos;
in float _current_time;
in flat int _show_vignette;
in flat float _var_vignette_size;
in flat float _var_vignette_smoothness;
in flat float _var_vignette_rounding;
in flat int _show_crt_tint;
in flat int _show_crt_warp;
in flat float _var_crt_warp_bend;
in flat int _show_crt_scanlines;
in flat float _var_crt_scanlines_intensity;

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

	vec4 final_color = Color;
	vec2 final_uv = TexCoord;

	// ----------------------------------------------------------------------------
	// CRT bending of image in the corners

	if( _show_crt_warp > 0 )
	{
		vec2 crt_uv = fx_crt_warp( final_uv, _var_crt_warp_bend );

		// adjust final_color to be fully black if outside the 0-1 range in UV coords

		if( crt_uv.x <= 0.0f || crt_uv.x >= 1.0f ) final_color = vec4(0,0,0,1);
		if( crt_uv.y <= 0.0f || crt_uv.y >= 1.0f ) final_color = vec4(0,0,0,1);

		final_uv = crt_uv;
	}

	// ----------------------------------------------------------------------------
	// rolling scan lines

	if( _show_crt_scanlines > 0 )
	{
		float s1 = fx_scanline( final_uv, 20.0f, -10.0f );
		float s2 = fx_scanline( final_uv, 2.0f, -3.0f );

		final_color = mix( final_color, vec4( s1 + s2 ), _var_crt_scanlines_intensity );
	}

	// ----------------------------------------------------------------------------
	// tint every other line to create cheap CRT effect

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
	// vignette

	if( _show_vignette > 0 )
	{
	  	final_color *= fx_vignette( final_uv, _var_vignette_size, _var_vignette_smoothness, _var_vignette_rounding );
	}

	// ----------------------------------------------------------------------------

	FragColor = texture( ourTexture, final_uv ) * final_color;
}
