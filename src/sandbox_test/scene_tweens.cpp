
#include "app_header.h"

using namespace war;

scene_tweens::scene_tweens()
{
	draws_completely_solid = true;
}

void scene_tweens::pushed()
{
	viewport_pivot = vec2::zero;
	g_engine->window.set_mouse_mode( mouse_mode::os );

	tween_tex = g_engine->find_asset<texture_asset>( "tex_particle_02" );

	tween_linear = tween( 0, 200, 3000, tween_type::pingpong, tween_via::linear );
	tween_sin = tween( 0, 200, 3000, tween_type::pingpong, tween_via::sinusoidal );
	tween_quad = tween( 0, 200, 3000, tween_type::pingpong, tween_via::quadratic );
	tween_circular = tween( 0, 200, 3000, tween_type::pingpong, tween_via::circular );
	tween_back = tween( 0, 200, 3000, tween_type::pingpong, tween_via::back );
	tween_bounce = tween( 0, 200, 3000, tween_type::pingpong, tween_via::bounce );
	tween_cubic = tween( 0, 200, 3000, tween_type::pingpong, tween_via::cubic );
	tween_elastic = tween( 0, 200, 3000, tween_type::pingpong, tween_via::elastic );
	tween_exponential = tween( 0, 200, 3000, tween_type::pingpong, tween_via::exponential );
	tween_quartic = tween( 0, 200, 3000, tween_type::pingpong, tween_via::quartic );
	tween_quintic = tween( 0, 200, 3000, tween_type::pingpong, tween_via::quintic );

	timer_01 = timer( 5000 );
	timer_02 = timer( 2500 );
}

void scene_tweens::draw()
{
	draw_tiled_background();
	scene::draw();
	render::draw_world_axis();
}

void scene_tweens::draw_ui()
{
	scene::draw_ui();
	draw_title( "Tweens Timers" );

	// ----------------------------------------------------------------------------

	{
		scoped_render_state;

		auto l_draw_label = [&] ( std::string_view desc, float& ypos )
		{
			render::state->color = make_color( 0, 0.35f );
			render::draw_filled_rect( rect( 96.f, ypos - 5.f, 200.f, 10.f ) );

			render::state->color = make_color( 0, 0.25f );
			render::draw_filled_rect( rect( 0.f, ypos - 7.f, ui_w, 14.f ) );

			render::state->color = make_color( 4, 1.f );
			render::state->scale = 1.f;
			render::state->align = align::vcenter;
			render::draw_string( desc, vec2( 12.f, ypos ) );

			ypos += 16.f;
		};

		float ypos = 40.f;

		{
			scoped_render_state;

			l_draw_label( "Linear:", ypos );
			l_draw_label( "Sinusoidal:", ypos );
			l_draw_label( "Quadratric:", ypos );
			l_draw_label( "Circular:", ypos );
			l_draw_label( "Bounce:", ypos );
			l_draw_label( "Back:", ypos );
			l_draw_label( "Cubic:", ypos );
			l_draw_label( "Elastic:", ypos );
			l_draw_label( "Exponential:", ypos );
			l_draw_label( "Quartic:", ypos );
			l_draw_label( "Quintic:", ypos );
		}
	}

	float ypos = 40.f;

	auto l_draw_marker = [&] ( float& ypos, tween& tweener )
	{
		render::state->color = color::white;
		render::state->scale = 2.f;
		render::draw_sprite( tween_tex, vec2( 96.f + *tweener, ypos ) );

		ypos += 16.f;
	};

	{
		scoped_render_state;

		l_draw_marker( ypos, tween_linear );
		l_draw_marker( ypos, tween_sin );
		l_draw_marker( ypos, tween_quad );
		l_draw_marker( ypos, tween_circular );
		l_draw_marker( ypos, tween_bounce );
		l_draw_marker( ypos, tween_back );
		l_draw_marker( ypos, tween_cubic );
		l_draw_marker( ypos, tween_elastic );
		l_draw_marker( ypos, tween_exponential );
		l_draw_marker( ypos, tween_quartic );
		l_draw_marker( ypos, tween_quintic );
	}

	render::draw_string( std::format( "Timer (will toggle in 5.0 seconds): {}", timer_01.is_elapsed() ), { 16.f, ui_h - 12.f } );
	render::draw_string( std::format( "Repeating Timer (2.5 second interval): {}", timer_02.get_elapsed() % 2 ? "ON" : "OFF" ), { 16.f, ui_h - 24.f } );
}
