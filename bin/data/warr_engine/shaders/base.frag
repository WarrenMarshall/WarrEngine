#version 420 core

layout (location = 0) out vec4 FragColor;

in vec3 Pos;
in vec2 TexCoord;
in vec4 Color;
in float Emissive;

uniform sampler2D Texture0;

void main()
{
   	FragColor = texture( Texture0, TexCoord ) * Color;
}
