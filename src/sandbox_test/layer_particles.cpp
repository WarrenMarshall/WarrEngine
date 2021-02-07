
#include "app_header.h"

// ----------------------------------------------------------------------------

layer_particles::layer_particles()
{
	draws_completely_solid = true;
}

void layer_particles::push()
{
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
			ec->rs_opt.color = w_color::red;
		}
		{
			auto ec = entity->add_component<ec_emitter>();
			ec->init( "em_stars" );
			ec->get_tform()->set_position( { 0.0f, -v_window_hh } );
		}
		{
			auto ec = entity->add_component<ec_primitive_shape>();
			ec->init( primitive_shape::point, 3.0f );
			ec->rs_opt.color = w_color::white;
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
			ec->init( primitive_shape::circle, 5.0f );
			ec->rs_opt.color = w_color::white;
		}
	}

	engine->window->set_mouse_mode( mouse_mode::os );
}

void layer_particles::update()
{
	w_layer::update();

	find_entity( H( "mouse_torch" ) )->get_tform()->set_position( INPUT->mouse_vwindow_pos );
}

void layer_particles::draw()
{
	RS->color = w_color::dark_grey;
	RENDER->draw_tiled( a_texture::find( "engine_tile_background_stripe" ), w_rect( 0.0f, 0.0f, ui_window_w, ui_window_h ) );

	RENDER->push();
	RS->align = align::centered;
	RS->scale = 2.0f;
	RS->color = w_color::white;
	RENDER->draw_string( "Particles", { ui_window_w / 2.0f, 16.0f } );
	RENDER->pop();

	w_layer::draw();
}
