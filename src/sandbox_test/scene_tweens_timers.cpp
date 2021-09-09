
#include "app_header.h"

using namespace war;

Scene_Tweens_Timers::Scene_Tweens_Timers()
{
	flags.blocks_further_drawing = true;
	flags.blocks_further_update = true;
}

void Scene_Tweens_Timers::pushed()
{
	Scene::pushed();

	viewport_pivot = Vec2::zero;
	g_engine->window.set_mouse_mode( e_mouse_mode::os );

	tween_tex = g_engine->find_asset<Texture_Asset>( "tex_particle_02" );

	tween_linear = Tween( 0, 200, 3000, e_tween_type::pingpong, e_tween_via::linear );
	tween_sin = Tween( 0, 200, 3000, e_tween_type::pingpong, e_tween_via::sinusoidal );
	tween_quad = Tween( 0, 200, 3000, e_tween_type::pingpong, e_tween_via::quadratic );
	tween_circular = Tween( 0, 200, 3000, e_tween_type::pingpong, e_tween_via::circular );
	tween_back = Tween( 0, 200, 3000, e_tween_type::pingpong, e_tween_via::back );
	tween_bounce = Tween( 0, 200, 3000, e_tween_type::pingpong, e_tween_via::bounce );
	tween_cubic = Tween( 0, 200, 3000, e_tween_type::pingpong, e_tween_via::cubic );
	tween_elastic = Tween( 0, 200, 3000, e_tween_type::pingpong, e_tween_via::elastic );
	tween_exponential = Tween( 0, 200, 3000, e_tween_type::pingpong, e_tween_via::exponential );
	tween_quartic = Tween( 0, 200, 3000, e_tween_type::pingpong, e_tween_via::quartic );
	tween_quintic = Tween( 0, 200, 3000, e_tween_type::pingpong, e_tween_via::quintic );

	timer_01 = Timer( 5000 );
	timer_02 = Timer( 2500 );
}

void Scene_Tweens_Timers::draw()
{
	draw_tiled_background();
	Scene::draw();
	Render::draw_world_axis();
}

void Scene_Tweens_Timers::draw_ui()
{
	Scene::draw_ui();
	draw_title( "Tweens Timers" );

	// ----------------------------------------------------------------------------

	{
		scoped_render_state;

		auto l_draw_label = [&] ( const std::string& desc, float& ypos )
		{
			Render::state->color = make_color( 0, 0.35f );
			Render::draw_filled_rect( Rect( 96.f, ypos - 5.f, 200.f, 10.f ) );

			Render::state->color = make_color( 0, 0.25f );
			Render::draw_filled_rect( Rect( 0.f, ypos - 7.f, ui_w, 14.f ) );

			Render::state->color = make_color( 4, 1.f );
			Render::state->scale = 1.f;
			Render::state->align = e_align::vcenter;
			Render::draw_string( desc, Vec2( 12.f, ypos ) );

			ypos += 16.f;
		};

		float_t ypos = 40.f;

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

	float_t ypos = 40.f;

	auto l_draw_marker = [&] ( float& ypos, Tween& tweener )
	{
		Render::state->color = Color::white;
		Render::state->scale = 2.f;
		Render::draw_sprite( tween_tex, Vec2( 96.f + *tweener, ypos ) );

		ypos += 16.f;
	};

	{
		scoped_render_state;

		Render::state->z += zdepth_nudge;

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

	Render::draw_string( std::format( "Timer (will toggle in 5.0 seconds): {}", timer_01.is_elapsed() ), { 16.f, ui_h - 12.f } );
	Render::draw_string( std::format( "Repeating Timer (2.5 second interval): {}", timer_02.get_elapsed() % 2 ? "ON" : "OFF" ), { 16.f, ui_h - 24.f } );
}
