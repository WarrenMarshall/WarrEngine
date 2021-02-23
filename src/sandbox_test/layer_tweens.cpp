
#include "app_header.h"

layer_tweens::layer_tweens()
{
	draws_completely_solid = true;
}

void layer_tweens::push()
{
	engine->window->set_mouse_mode( mouse_mode::os );

	tween_tex = a_texture::find( "tex_particle_02" );

	tween_linear = w_tween( 0, 200, 3000, tween_type::pingpong, tween_via::linear );
	tween_sin = w_tween( 0, 200, 3000, tween_type::pingpong, tween_via::sinusoidal );
	tween_quad = w_tween( 0, 200, 3000, tween_type::pingpong, tween_via::quadratic );
	tween_circular = w_tween( 0, 200, 3000, tween_type::pingpong, tween_via::circular );
	tween_back = w_tween( 0, 200, 3000, tween_type::pingpong, tween_via::back );
	tween_bounce = w_tween( 0, 200, 3000, tween_type::pingpong, tween_via::bounce );
	tween_cubic = w_tween( 0, 200, 3000, tween_type::pingpong, tween_via::cubic );
	tween_elastic = w_tween( 0, 200, 3000, tween_type::pingpong, tween_via::elastic );
	tween_exponential = w_tween( 0, 200, 3000, tween_type::pingpong, tween_via::exponential );
	tween_quartic = w_tween( 0, 200, 3000, tween_type::pingpong, tween_via::quartic );
	tween_quintic = w_tween( 0, 200, 3000, tween_type::pingpong, tween_via::quintic );

	timer_01 = w_timer( 5000 );
	timer_02 = w_timer( 2500 );

	{
		add_camera();
	}
}

void layer_tweens::draw()
{
	render_state =
	{
		.color = w_color::dark_teal
	};

	w_render::draw_tiled( a_texture::find( "engine_tile_background_stripe" ), w_rect( -viewport_hw, -viewport_hh, viewport_w, viewport_h ) );

	w_layer::draw();
}

void layer_tweens::draw_ui()
{
	w_layer::draw_ui();

	{
		scoped_render_state;

		render_state =
		{
			.align = align::centered,
			.color = w_color::white,
			.scale = 2.0f
		};

		w_render::draw_string( "Tweens / Timers", { ui_w / 2.0f, 16.0f } );
	}

	// ----------------------------------------------------------------------------

	auto l_draw_tweener = [&] ( std::string_view desc, float& ypos, w_tween& tweener )
	{
		render_state.color = w_color::black;
		render_state.color.a = 0.35f;
		w_render::draw_filled_rectangle( w_rect( 96.0f, ypos - 5.0f, 200.0f, 10.0f ) );

		render_state.color = w_color::black;
		render_state.color.a = 0.15f;
		w_render::draw_filled_rectangle( w_rect( 0.0f, ypos - 7.0f, ui_w, 14.0f ) );

		render_state.color = w_color::teal;
		render_state.color.a = 1.0f;
		render_state.scale = 1.0f;
		render_state.align = align::vcenter;
		w_render::draw_string( desc, w_pos( 12.0f, ypos ) );

		render_state.color = w_color::white;
		render_state.scale = 2.0f;
		w_render::draw_sprite( tween_tex, w_vec2( 96.0f + *tweener, ypos ) );

		ypos += 16.0f;
	};

	float ypos = 40.0f;

	{
		scoped_render_state;

		l_draw_tweener( "Linear:", ypos, tween_linear );
		l_draw_tweener( "Sinusoidal:", ypos, tween_sin );
		l_draw_tweener( "Quadratric:", ypos, tween_quad );
		l_draw_tweener( "Circular:", ypos, tween_circular );
		l_draw_tweener( "Bounce:", ypos, tween_bounce );
		l_draw_tweener( "Back:", ypos, tween_back );
		l_draw_tweener( "Cubic:", ypos, tween_cubic );
		l_draw_tweener( "Elastic:", ypos, tween_elastic );
		l_draw_tweener( "Exponential:", ypos, tween_exponential );
		l_draw_tweener( "Quartic:", ypos, tween_quartic );
		l_draw_tweener( "Quintic:", ypos, tween_quintic );
	}

	w_render::draw_string( fmt::format( "Timer (will toggle in 5.0 seconds): {}", timer_01.is_elapsed() ), { 16.0f, ui_h - 12.0f } );
	w_render::draw_string( fmt::format( "Repeating Timer (2.5 second interval): {}", timer_02.get_elapsed() % 2 ? "ON" : "OFF" ), { 16.0f, ui_h - 24.0f } );
}

bool layer_tweens::on_input_motion( const w_input_event* evt )
{
	if( evt->input_id == input_id::mouse )
	{
		// camera control
		if( INPUT->get_button_state( input_id::mouse_button_right ) == button_state::held )
		{
			if( evt->control_down )
			{
				get_camera()->get_transform()->add_angle_delta( w_coord::window_to_viewport_vec( evt->delta ).x );
			}
			else
			{
				get_camera()->get_transform()->add_pos_delta( w_coord::window_to_viewport_vec( evt->delta ) );
			}

			return true;
		}
	}

	return false;
}
