
#include "app_header.h"

using namespace war;

scene_default::scene_default()
{
	draws_completely_solid = true;
}

void scene_default::pushed()
{
	gradient = g_engine->find_asset<texture_asset>( "background_gradient" );
	tex_hello_world = g_engine->find_asset<texture_asset>( "tex_hello_world" );
	movement_tween = tween( -175, 175, 4000, tween_type::pingpong, tween_via::quadratic );
	scale_tween = tween( .5f, 5.f, 9000, tween_type::pingpong, tween_via::sinusoidal );
	tilt_tween = tween( -25.f, 25.f, 20000, tween_type::pingpong, tween_via::cubic );

	g_engine->window.set_mouse_mode( mouse_mode::os );
}

void scene_default::draw()
{
	scene::draw();


	//render::draw_world_axis();
	//render::draw_quad( gradient, rect( -viewport_hw, -viewport_hh, viewport_w, viewport_h ) );

	//render::state->angle = *tilt_tween;
	//render::state->scale = *scale_tween;
	//render::state->glow = 1.5f;
	//render::draw_sprite( tex_hello_world, vec2( *movement_tween, 0.f ) );

	scoped_render_state;

	render::state->color = make_color( color::red );
	render::draw_sprite( tex_hello_world, vec2( 0.f, 0.f ) );

	render::state->z += zdepth_nudge;
	render::state->color = make_color( color::green );
	render::draw_sprite( tex_hello_world, vec2( 16.f, 32.f ) );
}
