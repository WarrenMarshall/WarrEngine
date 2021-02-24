
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
		auto e = add_camera();
	}

	{
		auto e = add_entity<w_entity>();
		e->make_pickable();
		e->get_transform()->set_pos( { -100.0f, -75.0f } );
		e->get_transform()->set_scale( 2.0f );
		{
			auto ec = e->add_component<ec_primitive_shape>();
			ec->init( primitive_shape::filled_rectangle, w_rect( -16, -16, 32, 32 ) );
			ec->rs_opt.color = w_color::orange;
		}
	}

	{
		auto e = add_entity<w_entity>();
		e->make_pickable();
		e->get_transform()->set_pos( { 0.0f, 0.0f } );
		e->get_transform()->set_scale( 2.0f );
		{
			auto ec = e->add_component<ec_primitive_shape>();
			ec->init( primitive_shape::filled_circle, 16.0f );
			ec->rs_opt.color = w_color::teal;
		}
	}

	{
		auto e = add_entity<w_entity>();
		e->make_pickable();
		e->get_transform()->set_angle( 15.0f );
		e->get_transform()->set_pos( { +100.0f, +75.0f } );
		e->get_transform()->set_scale( 2.0f );
		{
			auto ec = e->add_component<ec_primitive_shape>();
			ec->init( primitive_shape::rectangle, w_rect( -32, -16, 64, 32 ) );
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
	render_state =
	{
		.color = w_color::dark_teal
	};
	w_render::draw_tiled( a_texture::find( "engine_tile_background_stripe" ), w_rect( -viewport_hw, -viewport_hh, viewport_w, viewport_h ) );

	w_layer::draw();

	w_render::draw_world_axis();
}

void layer_entity_picking::draw_ui()
{
	{
		scoped_render_state;

		render_state =
		{
			.align = align::centered,
			.color = w_color::white,
			.scale = 2.0f
		};

		w_render::draw_string( "Entity Picking", { ui_hw, 16.0f } );
	}

	w_layer::draw_ui();
}

bool layer_entity_picking::on_input_pressed( const w_input_event* evt )
{
	if( evt->input_id == input_id::mouse_button_left )
	{
		w_vec2 click_pos = w_coord::window_to_viewport_pos( engine->input->mouse_window_pos );
		auto pick_id = w_render::sample_pick_id_at( click_pos );

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
		if( engine->input->get_button_state( input_id::mouse_button_left ) == button_state::held )
		{
			w_vec2 delta = w_coord::window_to_world_vec( evt->delta, get_camera() );

			std::vector<w_entity*> selected_entities;
			get_selected( selected_entities );

			for( auto& e : selected_entities )
			{
				e->get_transform()->add_pos_delta( delta );
			}

			return true;
		}

		if( engine->input->get_button_state( input_id::mouse_button_right ) == button_state::held )
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
