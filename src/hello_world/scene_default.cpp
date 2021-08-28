
#include "app_header.h"

using namespace war;

scene_default::scene_default()
{
	flags.blocks_further_drawing = true;
}

void scene_default::pushed()
{
	scene::pushed();

	gradient = g_engine->find_asset<Texture_Asset>( "background_gradient" );
	tex_hello_world = g_engine->find_asset<Texture_Asset>( "tex_hello_world" );
	movement_tween = Tween( -175, 175, 4000, tween_type::pingpong, tween_via::quadratic );
	scale_tween = Tween( .5f, 5.f, 9000, tween_type::pingpong, tween_via::sinusoidal );
	tilt_tween = Tween( -25.f, 25.f, 20000, tween_type::pingpong, tween_via::cubic );

	g_engine->window.set_mouse_mode( mouse_mode::os );

}

void scene_default::draw()
{
	scene::draw();


	Render::draw_quad( gradient, Rect( -viewport_hw, -viewport_hh, viewport_w, viewport_h ) );
	Render::state->z += zdepth_nudge;

	Render::state->angle = *tilt_tween;
	Render::state->scale = *scale_tween;
	Render::draw_sprite( tex_hello_world, Vec2( *movement_tween, 0.f ) );
}
