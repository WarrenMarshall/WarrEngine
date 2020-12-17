#version 420 core

layout (location = 0) out vec4 FragColor;

in vec2 TexCoord;
in vec4 Color;
in vec3 Pos;

uniform sampler2D ourTexture;

void main()
{
   	FragColor = texture( ourTexture, TexCoord ) * Color;
}
