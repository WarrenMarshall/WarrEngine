#version 420 core

out vec4 out_fragment_color;

in vec3 fs_pos;
in vec2 fs_tex_coord;
in vec4 fs_color;
in float fs_emissive;

uniform sampler2D u_texture_0;

void main()
{
   	out_fragment_color = texture( u_texture_0, fs_tex_coord ) * fs_color;
}
