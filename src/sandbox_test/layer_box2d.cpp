
#include "app_header.h"

layer_box2d::layer_box2d()
{
	draws_completely_solid = true;
}

void layer_box2d::push()
{
	engine->window->set_mouse_mode( mouse_mode::os );

	{
		add_camera();
	}
}

void layer_box2d::draw()
{
	render_state =
	{
		.color = w_color::dark_teal
	};

	w_render::draw_tiled( a_texture::find( "engine_tile_background_stripe" ), w_rect( -viewport_hw, -viewport_hh, viewport_w, viewport_h ) );

	w_layer::draw();

	w_render::draw_world_axis();
}

void layer_box2d::draw_ui()
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

		w_render::draw_string( "Box2D Physics", { ui_w / 2.0f, 16.0f } );
	}
}

bool layer_box2d::on_input_motion( const w_input_event* evt )
{
	if( evt->input_id == input_id::mouse )
	{
		// camera control
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
