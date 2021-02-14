
#include "app_header.h"

// ----------------------------------------------------------------------------

layer_particles::layer_particles()
{
	draws_completely_solid = true;
}

void layer_particles::push()
{
	// camera
	{
		auto e = add_camera();
		//e->get_transform()->set_angle( 15.0f );
	}

	// background emitters
	{
		auto e = add_entity<w_entity>();

		e->get_transform()->set_pos( { v_window_hw, v_window_h } );
		{
			auto ec = e->add_component<ec_emitter>();
			ec->init( "em_fire_upwards" );
		}
		{
			auto ec = e->add_component<ec_primitive_shape>();
			ec->init( primitive_shape::filled_rectangle, w_rect( -v_window_hw, -2.0f, v_window_w, 4.0f ) );
			ec->rs_opt.color = w_color::light_green;
			ec->rs_opt.color->a = 0.25f;
		}
		{
			auto ec = e->add_component<ec_emitter>();
			ec->init( "em_stars" );
			ec->get_transform()->set_pos( { 0.0f, -v_window_hh } );
		}
		{
			auto ec = e->add_component<ec_primitive_shape>();
			ec->init( primitive_shape::point, 3.0f );
			ec->rs_opt.color = w_color::light_green;
			ec->get_transform()->set_pos( { 0.0f, -v_window_hh } );
		}
	}

	// torch emitter attached to mouse cursor
	{
		auto e = add_entity<w_entity>();
		e->tag = H( "mouse_torch" );
		e->get_transform()->set_pos( { v_window_hw, v_window_hh } );

		{
			auto ec = e->add_component<ec_emitter>();
			ec->init( "em_torch" );
		}
		{
			auto ec = e->add_component<ec_primitive_shape>();
			ec->init( primitive_shape::circle, 5.0f );
			ec->rs_opt.color = w_color::light_green;
			ec->rs_opt.color->a = 0.25f;
		}
	}

	engine->window->set_mouse_mode( mouse_mode::os );
}

void layer_particles::update()
{
	w_layer::update();

	if( follow_mouse )
	{
		// #refactor - this should get moved into a function that can convert between various coordinate spaces
		//			   in this case, vwindow -> camera
		auto pos = w_coord::virtual_pos_to_camera_pos( INPUT->mouse_vwindow_pos, get_camera() );

		find_entity( H( "mouse_torch" ) )->get_transform()->set_pos( pos );
	}
}

void layer_particles::draw()
{
	render_state =
	{
		.color = w_color::dark_teal
	};

	w_render::draw_tiled( a_texture::find( "engine_tile_background_stripe" ), w_rect( 0.0f, 0.0f, v_window_w, v_window_h ) );

	w_layer::draw();

	w_render::draw_world_axis();
}

void layer_particles::draw_ui()
{
	w_layer::draw_ui();

	{
		scoped_render_push_pop;

		render_state =
		{
			.align = align::centered,
			.color = w_color::white,
			.scale = 2.0f
		};

		w_render::draw_string( "Particles", { ui_window_w / 2.0f, 16.0f } );

		render_state =
		{
			.align = align::hcenter,
			.color = w_color::light_grey,
			.scale = 1.0f
		};

		w_render::draw_string( "R_DRAG / M_DRAG - move/rotate camera", w_pos( ui_window_hw, 200.0f ) );
		w_render::draw_string( "F - toggle follow mode for fire ball", w_pos( ui_window_hw, 208.0f ) );
	}
}

bool layer_particles::on_input_pressed( const w_input_event* evt )
{
	if( evt->input_id == input_id::key_f )
	{
		follow_mouse = !follow_mouse;

		return true;
	}

	return false;
}

bool layer_particles::on_input_motion( const w_input_event* evt )
{
	if( evt->input_id == input_id::mouse )
	{
		if( INPUT->get_button_state( input_id::mouse_button_right ) == button_state::held )
		{
			get_camera()->get_transform()->add_pos_delta( evt->vdelta );

			return true;
		}

		if( INPUT->get_button_state( input_id::mouse_button_middle ) == button_state::held )
		{
			get_camera()->get_transform()->add_angle_delta( evt->vdelta.x );

			return true;
		}
	}

	return false;
}
