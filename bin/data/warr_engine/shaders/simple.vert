#version 420 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inTexCoord;
layout (location = 2) in vec4 inColor;

uniform mat4 P; // projection matrix
uniform mat4 V; // camera matrix
uniform float in_current_time;

uniform int b_use_pixel_snapping = 1;

uniform int b_show_vignette = 0;
uniform float var_vignette_size = 1.9f;
uniform float var_vignette_smoothness = 0.6f;
uniform float var_vignette_rounding = 16.0f;

uniform int b_show_crt_tint = 0;

uniform int b_show_crt_warp = 0;
uniform float var_crt_warp_bend = 4.0f;

/*
uniform int b_show_crt_scanlines = 1;
uniform float var_crt_scanlines_intensity = 0.01f;
*/

out vec2 TexCoord;
out vec4 Color;
out vec3 Pos;
out float _current_time;
out flat int _show_vignette;
out flat float _var_vignette_size;
out flat float _var_vignette_smoothness;
out flat float _var_vignette_rounding;
out flat int _show_crt_tint;
out flat int _show_crt_warp;
out flat float _var_crt_warp_bend;
/*
out flat int _show_crt_scanlines;
out flat float _var_crt_scanlines_intensity;
*/

void main()
{
    // ----------------------------------------------------------------------------
    // optionally snap the vertex to the 1 grid for pixel perfect output

    Pos = inPos;

    if( b_use_pixel_snapping > 0 )
    {
        // snap the vertex to the pixel grid. this gives the game
        // a sharp pixel perfect look.

        vec3 snapped_pos;

        snapped_pos.x = floor( inPos.x + 0.5f );
        snapped_pos.y = floor( inPos.y + 0.5f );
        snapped_pos.z = floor( inPos.z + 0.5f );

        Pos = snapped_pos;
    }

    // apply matrix transforms to get the vertex to screen space.
	// "world space to camera space" * "camera space to projection space"

    gl_Position = P * V * vec4( Pos.x, Pos.y, Pos.z, 1.0 );

    // pass values to fragment shader

    TexCoord = inTexCoord;
    Color = inColor;
    _current_time = in_current_time;
    _show_vignette = b_show_vignette;
    _var_vignette_size = var_vignette_size;
    _var_vignette_smoothness = var_vignette_smoothness;
    _var_vignette_rounding = var_vignette_rounding;
    _show_crt_tint = b_show_crt_tint;
    _show_crt_warp = b_show_crt_warp;
    _var_crt_warp_bend = var_crt_warp_bend;
    /*
    _show_crt_scanlines = b_show_crt_scanlines;
    _var_crt_scanlines_intensity = var_crt_scanlines_intensity;
    */

}
