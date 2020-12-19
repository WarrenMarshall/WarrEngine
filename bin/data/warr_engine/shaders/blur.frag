#version 420 core

layout (location = 0) out vec4 FragColor;

in vec2 TexCoord;
in vec4 Color;
in vec3 Pos;

uniform sampler2D Texture0;

void main()
{
   	FragColor = 1 - texture( Texture0, TexCoord ) * Color;
   	FragColor.a = 1.0f;
}
