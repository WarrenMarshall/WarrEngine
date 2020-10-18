#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec4 Color;
in vec3 Pos;

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
	uv -= 0.5f;
	uv *= size;

	float amount = sqrt( pow( abs( uv.x ), edgeRounding ) + pow( abs( uv.y ), edgeRounding ) );

    amount = 1.0f - amount;

    return smoothstep( 0.0f, smoothness, amount );
}

void main()
{
	float crt_tint = 1.0f;

	if( (int( Pos.y ) % 2) > 0 )
	{
		crt_tint = 0.95f;
	}

	vec4 final_color = Color;
	vec2 uv = TexCoord;
	vec2 crt_uv = crt_coords( uv, 5.0f );

	if( crt_uv.x < 0.0f || crt_uv.x > 1.0f ) final_color = vec4(0,0,0,0);
	if( crt_uv.y < 0.0f || crt_uv.y > 1.0f ) final_color = vec4(0,0,0,0);

   	FragColor = texture( ourTexture, crt_uv );
   	FragColor *= final_color;
  	FragColor *= vignette( uv, 1.9f, 0.6f, 16.0f );
   	FragColor *= crt_tint;
}
