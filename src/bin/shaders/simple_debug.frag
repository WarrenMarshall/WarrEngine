#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in vec4 Color;

uniform sampler2D ourTexture;

void main()
{
	if( gl_FrontFacing )
	{
    	FragColor = texture( ourTexture, TexCoord ) * Color;
    }
    else
    {
    	// back facing polys will draw in red for debugging
    	FragColor = vec4( 1.0, 0.0, 0.0, 1.0 );
    }
}
