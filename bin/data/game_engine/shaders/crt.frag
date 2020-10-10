#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec4 Color;
in vec3 Pos;

uniform sampler2D ourTexture;

void main()
{
	float crt_tint = 1.0f;

	if( (int( Pos.y ) % 2) > 0 )
	{
		crt_tint = 0.95f;
	}

   	FragColor = texture( ourTexture, TexCoord ) * Color * crt_tint;
}
