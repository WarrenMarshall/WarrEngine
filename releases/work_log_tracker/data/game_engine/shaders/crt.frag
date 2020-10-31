#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec4 Color;
in vec3 Pos;
in float _time;
in float _use_vignette;

uniform sampler2D ourTexture;

// warp UVs so the screeb appears curved like an
// old crt screen.

vec2 crt_coords( vec2 uv, float bend )
{
	uv -= 0.5f;
	uv *= 2.0f;

	uv.x *= 1.0f + pow( abs(uv.y) / bend, 2.0f );
	uv.y *= 1.0f + pow( abs(uv.x) / bend, 2.0f );

	uv /= 2.0f;
	return uv + 0.5f;
}

float vignette( vec2 uv, float size, float smoothness, float edgeRounding )
{
	if( _use_vignette < 0.5f )
	{
		return 1.0f;
	}

	uv -= 0.5f;
	uv *= size;

	float amount = sqrt( pow( abs( uv.x ), edgeRounding ) + pow( abs( uv.y ), edgeRounding ) );

	amount = 1.0f - amount;

	return smoothstep( 0.0f, smoothness, amount );
}

float scanline( vec2 uv, float lines, float speed )
{
	return sin( uv.y * lines + _time * speed );
}

void main()
{
	float crt_tint = 1.0f;
	float crt_tint_inv = 0.0f;

	// tint every other line to create CRT scanline effect
	if( (int( Pos.y ) % 2) > 0 )
	{
		crt_tint = 0.975f;
		crt_tint_inv = 0.01f;
	}

	// generate baseline color and UV coords

	vec4 final_color = Color;
	vec2 uv = TexCoord;
	vec2 crt_uv = crt_coords( uv, 5.0f );

	// adjust final_color to be fully black if outside the 0-1 range in UV coords

	if( crt_uv.x < 0.0f || crt_uv.x > 1.0f ) final_color = vec4(0,0,0,0);
	if( crt_uv.y < 0.0f || crt_uv.y > 1.0f ) final_color = vec4(0,0,0,0);

	// -------------------
	// rolling scan lines
	// -------------------

	float s1 = scanline( uv, 20.0f, -10.0f );
	float s2 = scanline( uv, 2.0f, -3.0f );

	FragColor = mix( texture( ourTexture, crt_uv ), vec4( s1 + s2 ), 0.01f );

	// -------------------

   	FragColor *= final_color * crt_tint;

	// -------------------
	// vignette
	// -------------------

  	FragColor *= vignette( uv, 1.9f, 0.6f, 16.0f );
}
