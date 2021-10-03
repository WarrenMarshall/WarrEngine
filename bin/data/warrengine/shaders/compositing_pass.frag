#version 330 core

// ----------------------------------------------------------------------------
// compositing
// ----------------------------------------------------------------------------
//
// copies the pixels from the finished color attachment into a new color
// attachment while applying various effects.
//
// this is operating on the color attachments, which means it's only in
// game resolution. any effects added in this pass are relatively cheap since
// they are only hitting a small number of pixels.
//
// ----------------------------------------------------------------------------

layout (location = 0) out vec4 out_color_buffer;

in vec3 fs_pos;
in vec2 fs_tex_coord;
in vec4 fs_color;
in float fs_glow;
flat in int fs_texture_id;
flat in int fs_pick_id;

#pragma include "get_sample_from_texture.frag"

uniform float u_current_time;		// updated by the engine each frame
uniform float u_viewport_w = 320.0;
uniform float u_viewport_h = 240.0;
uniform float u_final_pixel_w = 320.0;
uniform float u_final_pixel_h = 240.0;

float PI = 3.14159;

// ----------------------------------------------------------------------------
// > chromatic aberration
// ----------------------------------------------------------------------------

uniform bool ub_chromatic_aberration = false;
uniform float u_chromatic_aberration_amount = 0.0025f;

vec4 fx_chromatic_aberration( vec4 output_color )
{
	if( ub_chromatic_aberration )
	{
		output_color.r = get_sample_from_texture( fs_texture_id, vec2(fs_tex_coord.x - u_chromatic_aberration_amount, fs_tex_coord.y - u_chromatic_aberration_amount)).r;
		output_color.b = get_sample_from_texture( fs_texture_id, vec2(fs_tex_coord.x + u_chromatic_aberration_amount, fs_tex_coord.y + u_chromatic_aberration_amount)).b;
	}

	return output_color;
}

// ----------------------------------------------------------------------------
// > color overlay - replaces the fragment color with a lerp. good for color fades, like to/from black.
// ----------------------------------------------------------------------------

uniform vec4 u_color_overlay = vec4( 0.0f, 0.0f, 0.0f, 0.0f );

vec4 fx_color_overlay( vec4 output_color )
{
	output_color = mix(
		output_color,
		vec4(u_color_overlay.r, u_color_overlay.g, u_color_overlay.b, 1.0f),
		u_color_overlay.a
	);

	return output_color;
}

// ----------------------------------------------------------------------------
// > color tint - gets added to the fragment color. good for damage indicators
// ----------------------------------------------------------------------------

uniform vec4 u_colorize = vec4( 1.0f, 1.0f, 1.0f, 0.0f );

vec4 fx_colorize( vec4 output_color )
{
	output_color.rgb += u_colorize.rgb * u_colorize.a;

	return output_color;
}

// ----------------------------------------------------------------------------
// > film grain - random grainy noise
// ----------------------------------------------------------------------------

uniform bool ub_film_grain = false;
uniform float u_film_grain_intensity = 0.05f;
uniform float u_film_grain_amount = 0.0f;		// updated by the engine each frame

float fx_film_grain_random( vec2 p )
{
	vec2 K1 = vec2(
		23.14069263277926,	// e^pi (Gelfond's constant)
		2.665144142690225	// 2^sqrt(2) (Gelfond�Schneider constant)
	);

	return fract( cos( dot( p, K1 ) ) * 12345.6789 );
}

vec4 fx_film_grain( vec4 output_color )
{
	if( ub_film_grain )
	{
		vec4 color = output_color;
		vec2 uvRandom = fs_tex_coord;
		uvRandom.y *= fx_film_grain_random( vec2( uvRandom.y, u_film_grain_amount ) );
		color.rgb += fx_film_grain_random( uvRandom ) * u_film_grain_intensity;
		output_color = vec4( color.rgb, output_color.a );
	}

	return output_color;
}

// ----------------------------------------------------------------------------
// > crt line tint - darken every second line
// ----------------------------------------------------------------------------

uniform bool ub_crt_tint = false;

// a scaling factor for the number of crt lines (2.0 = twice as many, 0.5 = half as many).
// it's subjective depending on your resolution so play with it.
uniform float u_crt_tint_scaling_factor = 2.0;

// how intense the light/dark shift should be between the crt lines. (0 - invisible, 1 - harsh)
uniform float u_crt_tint_intensity = 0.1;

vec4 fx_crt_tint_lines( vec4 output_color )
{
	if( ub_crt_tint )
	{
		float line_count = ( u_viewport_h / 2.0 ) / u_crt_tint_scaling_factor;
		float frequency = line_count * 2.0 * PI;

		float screen_v = gl_FragCoord.y / u_viewport_h;
		float crt_tint = 1.0 - u_crt_tint_intensity * ( sin( frequency * screen_v ) * 0.5 + 0.5 );

		output_color *= crt_tint;
	}

	return output_color;
}

// ----------------------------------------------------------------------------
// > crt scan lines - rolling vertically on the display
// ----------------------------------------------------------------------------

uniform bool ub_crt_scanlines = false;
uniform float u_crt_scanlines_intensity = 0.005f;

float fx_crt_scanlines_wk( vec2 uv, float lines, float speed )
{
	return sin( uv.y * lines + u_current_time * speed );
}

vec4 fx_crt_scanlines( vec4 output_color )
{
	if( ub_crt_scanlines )
	{
		float s1 = fx_crt_scanlines_wk( fs_tex_coord, 200.0f, -10.0f );
		float s2 = fx_crt_scanlines_wk( fs_tex_coord, 20.0f, -3.0f );

		output_color = mix( output_color, vec4( s1 + s2 ), u_crt_scanlines_intensity );
	}

	return output_color;
}

// ----------------------------------------------------------------------------
// > vignette
// ----------------------------------------------------------------------------

uniform bool ub_vignette = false;
uniform float u_vignette_size = 1.9f;
uniform float u_vignette_smoothness = 0.75f;
uniform float u_vignette_rounding = 12.0f;

float fx_vignette_wk( vec2 uv, float size, float smoothness, float edgeRounding )
{
	uv -= 0.5f;
	uv *= size;

	float amount = sqrt( pow( abs( uv.x ), edgeRounding ) + pow( abs( uv.y ), edgeRounding ) );

	amount = 1.0f - amount;

	return smoothstep( 0.0f, smoothness, amount );
}

vec4 fx_vignette( vec4 output_color )
{
	if( ub_vignette )
	{
		output_color *= fx_vignette_wk( fs_tex_coord, u_vignette_size, u_vignette_smoothness, u_vignette_rounding );
	}

	return output_color;
}

// ----------------------------------------------------------------------------
// > desaturation
// ----------------------------------------------------------------------------

uniform bool ub_desaturation = false;
uniform float u_desaturation_amount = 0.25f;

vec4 fx_desaturation( vec4 output_color )
{
	if( ub_desaturation )
	{
		vec3 rgb = vec3( output_color.r, output_color.g, output_color.b );
		float T = u_desaturation_amount;

		output_color = vec4( mix( vec3( dot( rgb, vec3( 0.2125, 0.7154, 0.0721 ) ) ), rgb, T ), output_color.a );
	}

	return output_color;
}

// ----------------------------------------------------------------------------
// > fx_pixelate_to_target_res
// ----------------------------------------------------------------------------

vec2 fx_pixelate_to_target_res( vec2 output_uvs )
{
	// #pixelate - not working properly yet but it feels like a feature worth having eventually
	//output_uvs.x = round( output_uvs.x * u_final_pixel_w ) / u_final_pixel_w;
	//output_uvs.y = round( output_uvs.y * u_final_pixel_h ) / u_final_pixel_h;

	return output_uvs;
}

// ----------------------------------------------------------------------------

void main()
{
	vec2 final_uvs = fx_pixelate_to_target_res( fs_tex_coord );

	out_color_buffer = get_sample_from_texture( fs_texture_id, final_uvs ) * fs_color;

	// post process stack

	out_color_buffer = fx_chromatic_aberration( out_color_buffer );
	out_color_buffer = fx_desaturation( out_color_buffer );
	out_color_buffer = fx_colorize( out_color_buffer );
	out_color_buffer = fx_color_overlay( out_color_buffer );
	out_color_buffer = fx_crt_tint_lines( out_color_buffer );
	out_color_buffer = fx_crt_scanlines( out_color_buffer );
	out_color_buffer = fx_film_grain( out_color_buffer );
	out_color_buffer = fx_vignette( out_color_buffer );
}
