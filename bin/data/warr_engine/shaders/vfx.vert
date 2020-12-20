#version 420 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inTexCoord;
layout (location = 2) in vec4 inColor;
layout (location = 3) in float inEmissive;

// ----------------------------------------------------------------------------

uniform mat4 P; // projection matrix
uniform mat4 V; // camera matrix

uniform float in_current_time;

uniform int b_show_vignette = 0;
uniform float var_vignette_size = 1.9f;
uniform float var_vignette_smoothness = 0.75f;
uniform float var_vignette_rounding = 16.0f;

uniform int b_show_crt_tint = 0;

uniform int b_show_crt_warp = 0;
uniform float var_crt_warp_bend = 4.0f;

uniform int b_show_crt_scanlines = 0;
uniform float var_crt_scanlines_intensity = 0.01f;

uniform int b_show_chromatic_abberation = 0;
uniform float var_chromatic_abberation_amount = 0.0025f;

uniform int b_show_desaturation = 0;
uniform float var_desaturation_amount = 0.15f;

uniform int b_show_inverted = 0;

uniform vec4 var_color_tint = vec4(  1.0f, 1.0f, 1.0f, 0.0f );

uniform vec4 var_color_overlay = vec4(  0.0f, 0.0f, 0.0f, 0.0f );

// ----------------------------------------------------------------------------

out vec3 Pos;
out vec2 TexCoord;
out vec4 Color;
out float Emissive;

out flat float _current_time;
out flat int _show_vignette;
out flat float _var_vignette_size;
out flat float _var_vignette_smoothness;
out flat float _var_vignette_rounding;
out flat int _show_crt_tint;
out flat int _show_crt_warp;
out flat float _var_crt_warp_bend;
out flat int _show_crt_scanlines;
out flat float _var_crt_scanlines_intensity;
out flat int _show_chromatic_abberation;
out flat float _var_chromatic_abberation_amount;
out flat int _show_desaturation;
out flat float _var_desaturation_amount;
out flat int _show_inverted;
out flat vec4 _var_color_tint;
out flat vec4 _var_color_overlay;

// ----------------------------------------------------------------------------

void main()
{
    Pos = inPos;

    // apply matrix transforms to get the vertex to screen space.
	// "world space to camera space" * "camera space to projection space"

    gl_Position = P * V * vec4( Pos.x, Pos.y, Pos.z, 1.0 );

    // pass values to fragment shader

    TexCoord = inTexCoord;
    Color = inColor;
    Emissive = inEmissive;

    _current_time = in_current_time;
    _show_vignette = b_show_vignette;
    _var_vignette_size = var_vignette_size;
    _var_vignette_smoothness = var_vignette_smoothness;
    _var_vignette_rounding = var_vignette_rounding;
    _show_crt_tint = b_show_crt_tint;
    _show_crt_warp = b_show_crt_warp;
    _var_crt_warp_bend = var_crt_warp_bend;
    _show_crt_scanlines = b_show_crt_scanlines;
    _var_crt_scanlines_intensity = var_crt_scanlines_intensity;
    _show_chromatic_abberation = b_show_chromatic_abberation;
    _var_chromatic_abberation_amount = var_chromatic_abberation_amount;
    _show_desaturation = b_show_desaturation;
    _var_desaturation_amount = var_desaturation_amount;
    _show_inverted = b_show_inverted;
    _var_color_tint = var_color_tint;
    _var_color_overlay = var_color_overlay;
}
