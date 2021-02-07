#version 420 core

out vec4 out_fragment_color;

in vec3 fs_pos;
in vec2 fs_tex_coord;
in vec4 fs_color;
in float fs_glow;
flat in float fs_texture_id;
flat in float fs_pick_id;

uniform sampler2D u_textures[__max_texture_image_units__];

void main()
{
	int texture_idx = int(fs_texture_id);

	out_fragment_color = texture( u_textures[texture_idx], fs_tex_coord ) * fs_color;
}
