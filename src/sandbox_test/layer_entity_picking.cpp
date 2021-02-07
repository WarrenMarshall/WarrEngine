
#include "app_header.h"

// ----------------------------------------------------------------------------

layer_entity_picking::layer_entity_picking()
{
	draws_completely_solid = true;
}

void layer_entity_picking::push()
{
	engine->window->set_mouse_mode( mouse_mode::os );

	{
		auto e = add_entity<w_entity>();
		e->pick_id = 0.01f;
		e->get_tform()->set_position( { v_window_hw - 100.0f, 85.0f } );
		{
			auto ec = e->add_component<ec_primitive_shape>();
			ec->init( primitive_shape::filled_rectangle, w_rect( -16, -16, 32, 32 ) );
			ec->rs_opt.color = w_color::orange;
		}
	}

	{
		auto e = add_entity<w_entity>();
		e->pick_id = 0.02f;
		e->get_tform()->set_position( { v_window_hw, 125.0f } );
		{
			auto ec = e->add_component<ec_primitive_shape>();
			ec->init( primitive_shape::filled_circle, 16.0f );
			ec->rs_opt.color = w_color::light_green;
		}
	}

	{
		auto e = add_entity<w_entity>();
		e->pick_id = 0.03f;
		e->get_tform()->set_angle( 15.0f );
		e->get_tform()->set_position( { v_window_hw + 100.0f, 165.0f } );
		{
			auto ec = e->add_component<ec_primitive_shape>();
			ec->init( primitive_shape::rectangle, w_rect( -16, -16, 32, 32 ) );
			ec->rs_opt.color = w_color::yellow;
		}
	}
}

void layer_entity_picking::draw()
{
	RS->color = w_color::dark_teal;
	RENDER->draw_tiled( a_texture::find( "engine_tile_background_stripe" ), w_rect( 0.0f, 0.0f, ui_window_w, ui_window_h ) );

	RENDER_BLOCK
	(
		RS->align = align::centered;
		RS->scale = 2.0f;
		RS->color = w_color::white;
		RENDER->draw_string( "Entity Picking", { ui_window_w / 2.0f, 16.0f } );
	)

	w_layer::draw();

	RENDER->draw_world_axis();
}

void layer_entity_picking::draw_ui()
{
	w_layer::draw_ui();
}

bool layer_entity_picking::on_input_pressed( const w_input_event* evt )
{
	if( evt->input_id == input_id::mouse_button_left )
	{
		w_vec2 click_pos = INPUT->mouse_vwindow_pos;

		float pick_id = engine->sample_pick_id( click_pos );

		return true;
	}

	return false;
}
