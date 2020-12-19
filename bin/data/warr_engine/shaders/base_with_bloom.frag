#version 420 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BloomColor;

in vec2 TexCoord;
in vec4 Color;
in vec3 Pos;

uniform sampler2D Texture0;

void main()
{
    // ------------------------------------
	// normal rendering

   	FragColor = texture( Texture0, TexCoord ) * Color;

    // ------------------------------------
   	// bloom rendering, picking out the bright/hot pixels only

    float brightness = dot( FragColor.rgb, vec3( 0.2126f, 0.7152f, 0.0722f ) );

    if( brightness > 0.85f )
    {
        BloomColor = FragColor;
    }
    else
    {
        BloomColor = vec4( 0.0f, 0.0f, 0.0f, 1.0f );
    }
}
