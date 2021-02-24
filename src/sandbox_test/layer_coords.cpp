
#include "app_header.h"

// ----------------------------------------------------------------------------

layer_coords::layer_coords()
{
	draws_completely_solid = true;
}

void layer_coords::push()
{
	engine->window->set_mouse_mode( mouse_mode::custom );

	{
		add_camera();
	}

	{
		auto e = add_entity<w_entity>();
		e->tag = H( "crosshair" );
		auto ec = e->add_component<ec_sprite>();
		ec->init( "tex_crosshair" );
	}

}

void layer_coords::draw()
{
	{
		scoped_render_state;
		render_state.color = w_color::dark_teal;

		w_render::draw_tiled( a_texture::find( "engine_tile_background_stripe" ), w_rect( -viewport_hw, -viewport_hh, viewport_w, viewport_h ) );
	}

	w_layer::draw();

	w_render::draw_world_axis();
}

void layer_coords::draw_ui()
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

		w_render::draw_string( "Coordinate Systems", { ui_w / 2.0f, 16.0f } );
	}

	// label next to the crosshair
	{
		auto e = find_entity( H( "crosshair" ) );

		auto vpos = w_coord::world_to_viewport_pos( e->get_transform()->pos, get_camera() );
		auto uipos = w_coord::viewport_to_ui_pos( vpos );

		scoped_render_state;
		render_state.align = align::vcenter;
		render_state.color = w_color::orange;
		render_state.color.a = 0.75f;
		w_render::draw_string( "<< Your crosshair", uipos + w_vec2( 8.0f, 0.0f ) );
	}
}

bool layer_coords::on_input_motion( const w_input_event* evt )
{
	switch( evt->input_id )
	{
		case input_id::mouse:
		{
			if( engine->input->get_button_state( input_id::mouse_button_left ) == button_state::held )
			{
				auto e = find_entity( H( "crosshair" ) );

				if( evt->control_down )
				{
					e->get_transform()->add_angle_delta( w_coord::window_to_viewport_vec( evt->delta ).x );
				}
				else
				{
					e->get_transform()->set_pos( w_coord::window_to_world_pos( evt->mouse_pos, get_camera() ) );
				}

				return true;
			}

			// camera control
			if( engine->input->get_button_state( input_id::mouse_button_right ) == button_state::held )
			{
				if( evt->control_down )
				{
					get_camera()->get_transform()->add_angle_delta( w_coord::window_to_viewport_vec( evt->delta ).x );
				}
				else if( evt->alt_down )
				{
					get_camera()->get_transform()->add_scale_delta( w_coord::window_to_viewport_vec( evt->delta ).x * 0.1f );
				}
				else
				{
					get_camera()->get_transform()->add_pos_delta( w_coord::window_to_viewport_vec( evt->delta ) );
				}

				return true;
			}
		}
		break;

		case input_id::mouse_wheel:
		{
			get_camera()->get_transform()->add_scale_delta( w_coord::window_to_viewport_vec( evt->delta ).y * 0.1f );
		}
		break;
	}

	return false;
}
