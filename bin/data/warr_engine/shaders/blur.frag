#version 420 core

layout (location = 0) out vec4 FragColor;

in vec2 TexCoord;
in vec4 Color;
in vec3 Pos;

uniform sampler2D Texture0;

// implements a gaussian blur effect either in the horizontal
// or vertical directions, based on the "horizontal" variable.
//
// yoinked from : https://learnopengl.com/Advanced-Lighting/Bloom

uniform bool horizontal;
uniform float weight[5] = float[]
	(
		0.227027,
		0.1945946,
		0.1216216,
		0.054054,
		0.016216
	);

void main()
{
	// gets size of single texel
    vec2 tex_offset = 1.0 / textureSize(Texture0, 0);

	// current fragment's contribution
    vec3 result = texture(Texture0, TexCoord).rgb * weight[0];

    if( horizontal )
    {
        for( int i = 1; i < 5; ++i )
        {
            result += texture(Texture0, TexCoord + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(Texture0, TexCoord - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for( int i = 1; i < 5; ++i )
        {
            result += texture(Texture0, TexCoord + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            result += texture(Texture0, TexCoord - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        }
    }

    FragColor = vec4(result, 1.0);
}
