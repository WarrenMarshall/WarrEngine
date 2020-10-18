#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec4 Color;
in vec3 Pos;

uniform sampler2D ourTexture;

vec2 crt_coords(vec2 uv, float bend)
{
	uv -= 0.5;
    uv *= 2.;
    uv.x *= 1. + pow(abs(uv.y)/bend, 2.);
    uv.y *= 1. + pow(abs(uv.x)/bend, 2.);

    uv /= 2.;
    return uv + .5;
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
	vec2 crt_uv = crt_coords( uv, 6.0f );

	if( crt_uv.x < 0.0f || crt_uv.x > 1.0f ) final_color = vec4(0,0,0,0);
	if( crt_uv.y < 0.0f || crt_uv.y > 1.0f ) final_color = vec4(0,0,0,0);

   	FragColor = texture( ourTexture, crt_uv );
   	FragColor *= final_color;
   	FragColor *= crt_tint;
}
