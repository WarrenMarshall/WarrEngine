
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
		e->tag = H( "main_camera" );
		e->get_tform()->set_position( { ui_window_hw, ui_window_hh } );
	}

	{
		auto e = add_entity<w_entity>();
		e->pick_id = 1.0f;
		e->get_tform()->set_position( { -100.0f, -75.0f } );
		{
			auto ec = e->add_component<ec_primitive_shape>();
			ec->init( primitive_shape::filled_rectangle, w_rect( -16, -16, 32, 32 ) );
			ec->rs_opt.color = w_color::orange;
		}
	}

	{
		auto e = add_entity<w_entity>();
		e->pick_id = 2.0f;
		e->get_tform()->set_position( { 0.0f, 0.0f } );
		{
			auto ec = e->add_component<ec_primitive_shape>();
			ec->init( primitive_shape::filled_circle, 16.0f );
			ec->rs_opt.color = w_color::red;
		}
	}

	{
		auto e = add_entity<w_entity>();
		e->pick_id = 3.0f;
		e->get_tform()->set_angle( 15.0f );
		e->get_tform()->set_position( { +100.0f, +75.0f } );
		{
			auto ec = e->add_component<ec_primitive_shape>();
			ec->init( primitive_shape::filled_rectangle, w_rect( -32, -16, 64, 32 ) );
			ec->rs_opt.color = w_color::dark_green;
		}
		{
			auto ec = e->add_component<ec_primitive_shape>();
			ec->init( primitive_shape::filled_rectangle, w_rect( -24, -12, 48, 24 ) );
			ec->rs_opt.color = w_color::green;
		}
	}
}

void layer_entity_picking::draw()
{
	RS->color = w_color::dark_teal;
	RENDER->draw_tiled( a_texture::find( "engine_tile_background_stripe" ), w_rect( -ui_window_hw, -ui_window_hh, ui_window_w, ui_window_h ) );

	RENDER->draw_world_axis();

	w_layer::draw();
}

void layer_entity_picking::draw_ui()
{
	RENDER_BLOCK
	(
		RS->align = align::centered;
		RS->scale = 2.0f;
		RS->color = w_color::white;
		RENDER->draw_string( "Entity Picking", { ui_window_w / 2.0f, 16.0f } );
	)

	RENDER_BLOCK
	(
		RS->color = w_color::white;
		RS->align = align::hcenter;
		RENDER->draw_string( "RIGHT_DRAG to move camera", w_pos( v_window_hw, 200.0f ) );
		RENDER->draw_string( "LEFT_CLICK to select entity", w_pos( v_window_hw, 208.0f ) );
		RENDER->draw_string( "LEFT_DRAG to move selected entity", w_pos( v_window_hw, 216.0f ) );
	)

	w_layer::draw_ui();
}

bool layer_entity_picking::on_input_pressed( const w_input_event* evt )
{
	if( evt->input_id == input_id::mouse_button_left )
	{
		w_vec2 click_pos = INPUT->mouse_vwindow_pos;
		auto pick_id = RENDER->sample_pick_id_at( click_pos );

		deselect_all();
		select_by_pick_id( pick_id );

		for( auto& e : entities )
		{
			e->rs_opt.glow = e->is_selected ? 0.25f : 0.0f;
		}

		return true;
	}

	return false;
}

bool layer_entity_picking::on_input_motion( const w_input_event* evt )
{
	if( evt->input_id == input_id::mouse )
	{
		if( INPUT->get_button_state( input_id::mouse_button_left ) == button_state::held )
		{
			std::vector<w_entity*> sels;
			get_selected( sels );

			for( auto& e : sels )
			{
				e->get_tform()->add_position_delta( evt->vdelta );
			}

			return true;
		}

		if( INPUT->get_button_state( input_id::mouse_button_right ) == button_state::held )
		{
			get_camera()->get_tform()->add_position_delta( evt->vdelta );

			return true;
		}
	}

	return false;
}
