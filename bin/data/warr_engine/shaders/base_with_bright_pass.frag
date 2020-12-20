#version 420 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec3 Pos;
in vec2 TexCoord;
in vec4 Color;
in float Emissive;

uniform sampler2D Texture0;

void main()
{
    // ------------------------------------
	// first color buffer
    //
    // - regular rendering

   	FragColor = texture( Texture0, TexCoord ) * Color;

    // ------------------------------------
    // second color buffer
    //
   	// - bright rendering
    // - filters out the pixels that should contribute to bloom
    // - this is pixels that are overbright,
    //   or have an explicit emissive value assigned to them

    float brightness = dot( FragColor.rgb, vec3( 0.2126f, 0.7152f, 0.0722f ) );

    if( Emissive > 0.0f )
    {
        float emissive_power = 1 + Emissive;
        vec3 emissive_color = vec3( FragColor.r, FragColor.g, FragColor.b ) * emissive_power;
        BrightColor = vec4( emissive_color, FragColor.a );
    }
    else if( brightness > 1.0f )
    {
       BrightColor = FragColor;
    }
    else
    {
        BrightColor = vec4( 0.0f, 0.0f, 0.0f, 1.0f );
    }
}
