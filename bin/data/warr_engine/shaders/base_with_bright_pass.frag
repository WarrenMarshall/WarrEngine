#version 420 core

layout (location = 0) out vec4 out_fragment_color;
layout (location = 1) out vec4 out_bright_color;

in vec3 fs_pos;
in vec2 fs_tex_coord;
in vec4 fs_color;
in float fs_emissive;

uniform sampler2D u_texture_0;

void main()
{
    // ------------------------------------
	// first color buffer
    //
    // - regular rendering

   	out_fragment_color = texture( u_texture_0, fs_tex_coord ) * fs_color;

    // ------------------------------------
    // second color buffer
    //
   	// - bright rendering
    // - filters out the pixels that should contribute to bloom
    // - this is pixels that are overbright,
    //   or have an explicit fs_emissive value assigned to them

    float brightness = dot( out_fragment_color.rgb, vec3( 0.2126f, 0.7152f, 0.0722f ) );

    if( fs_emissive > 0.0f )
    {
        float strength = 1 + fs_emissive;
        vec3 color = vec3( out_fragment_color.r, out_fragment_color.g, out_fragment_color.b ) * strength;
        out_bright_color = vec4( color, out_fragment_color.a );
    }
    else if( brightness > 1.0f )
    {
       out_bright_color = out_fragment_color;
    }
    else
    {
        out_bright_color = vec4( 0.0f, 0.0f, 0.0f, 1.0f );
    }
}
