
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
		auto entity = add_entity<w_entity>();
		entity->tag = H( "main_camera" );
	}

	// background emitters
	{
		auto entity = add_entity<w_entity>();

		entity->get_tform()->set_position( { v_window_hw, v_window_h } );
		{
			auto ec = entity->add_component<ec_emitter>();
			ec->init( "em_fire_upwards" );
		}
		{
			auto ec = entity->add_component<ec_primitive_shape>();
			ec->init( primitive_shape::filled_rectangle, w_rect( -v_window_hw, -2.0f, v_window_w, 4.0f ) );
			ec->rs_opt.color = w_color::light_green;
			ec->rs_opt.color->a = 0.25f;
		}
		{
			auto ec = entity->add_component<ec_emitter>();
			ec->init( "em_stars" );
			ec->get_tform()->set_position( { 0.0f, -v_window_hh } );
		}
		{
			auto ec = entity->add_component<ec_primitive_shape>();
			ec->init( primitive_shape::point, 3.0f );
			ec->rs_opt.color = w_color::light_green;
			ec->get_tform()->set_position( { 0.0f, -v_window_hh } );
		}
	}

	// torch emitter attached to mouse cursor
	{
		auto entity = add_entity<w_entity>();
		entity->tag = H( "mouse_torch" );

		{
			auto ec = entity->add_component<ec_emitter>();
			ec->init( "em_torch" );
		}
		{
			auto ec = entity->add_component<ec_primitive_shape>();
			ec->init( primitive_shape::filled_circle, 5.0f );
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
		find_entity( H( "mouse_torch" ) )->get_tform()->set_position( INPUT->mouse_vwindow_pos );
	}
}

void layer_particles::draw()
{
	RS->color = w_color::dark_grey;
	RENDER->draw_tiled( a_texture::find( "engine_tile_background_stripe" ), w_rect( 0.0f, 0.0f, ui_window_w, ui_window_h ) );

	RENDER_BLOCK
	(
		RS->align = align::centered;
		RS->scale = 2.0f;
		RS->color = w_color::white;
		RENDER->draw_string( "Particles", { ui_window_w / 2.0f, 16.0f } );
	)

	w_layer::draw();
}

void layer_particles::draw_ui()
{
	w_layer::draw_ui();

	RENDER_BLOCK
	(
		RS->color = w_color::white;
		RS->align = align::hcenter;
		RENDER->draw_string( "RIGHT_DRAG to move camera", w_pos( v_window_hw, 200.0f ) );
		RENDER->draw_string( "F - toggle follow mode for fire ball", w_pos( v_window_hw, 208.0f ) );
	)
}

bool layer_particles::on_input_pressed( const w_input_event* evt )
{
	if( evt->input_id == input_id::key_f )
	{
// #warren
// 		this messes up if you move the camera. so we need some sort of
// 		vwindow to world space conversion function.
//
// 		should this be expanded into a general X-space to X-space series of functions?

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
			get_camera()->get_tform()->add_position_delta( evt->vdelta );

			return true;
		}
	}

	return false;
}
