#version 420 core

out vec4 out_fragment_color;

in vec3 fs_pos;
in vec2 fs_tex_coord;
in vec4 fs_color;
in float fs_emissive;

uniform sampler2D u_texture_0;

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
    vec2 tex_offset = 1.0 / textureSize(u_texture_0, 0);

	// current fragment's contribution
    vec3 result = texture(u_texture_0, fs_tex_coord).rgb * weight[0];

    if( horizontal )
    {
        for( int i = 1; i < 5; ++i )
        {
            result += texture(u_texture_0, fs_tex_coord + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(u_texture_0, fs_tex_coord - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for( int i = 1; i < 5; ++i )
        {
            result += texture(u_texture_0, fs_tex_coord + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            result += texture(u_texture_0, fs_tex_coord - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        }
    }

    out_fragment_color = vec4(result, 1.0);
}
