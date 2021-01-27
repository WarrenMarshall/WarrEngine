#version 420 core

out vec4 out_fragment_color;

in vec3 fs_pos;
in vec2 fs_tex_coord;
in vec4 fs_color;
in float fs_glow;
in float fs_texture_id;

uniform sampler2D u_textures[__max_texture_image_units__];

// implements a gaussian blur effect either in the horizontal
// or vertical directions, based on the "horizontal" variable.
//
// yoinked from : https://learnopengl.com/Advanced-Lighting/Bloom

uniform bool horizontal;
uniform float weight[9] = float[]
	(
		0.227027 / 2.0f,
		0.227027 / 2.0f,
		0.1945946 / 2.0f,
		0.1945946 / 2.0f,
		0.1216216,
		0.054054 / 2.0f,
		0.054054 / 2.0f,
		0.016216 / 2.0f,
		0.016216 / 2.0f
	);

void main()
{
	int texture_idx = int(fs_texture_id);

	// gets size of single texel
	vec2 tex_offset = 1.0 / textureSize(u_textures[texture_idx], 0);

	// current fragment's contribution
	vec3 result = texture(u_textures[texture_idx], fs_tex_coord).rgb * weight[0];

	if( horizontal )
	{
		for( int i = 1; i < 9; ++i )
		{
			result += texture(u_textures[texture_idx], fs_tex_coord + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
			result += texture(u_textures[texture_idx], fs_tex_coord - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
		}
	}
	else
	{
		for( int i = 1; i < 9; ++i )
		{
			result += texture(u_textures[texture_idx], fs_tex_coord + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
			result += texture(u_textures[texture_idx], fs_tex_coord - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
		}
	}

	out_fragment_color = vec4(result, 1.0) * 1.05f;
}
