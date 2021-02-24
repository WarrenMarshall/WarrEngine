
#include "app_header.h"

// ----------------------------------------------------------------------------

layer_meshes::layer_meshes()
{
	draws_completely_solid = true;

	mesh_rotator = w_tween( 0, 360, 5000, tween_type::pingpong, tween_via::bounce );
	mesh_scaler = w_tween( 1.0f, 3.5f, 3000, tween_type::pingpong, tween_via::elastic );
}

void layer_meshes::push()
{
	{
		auto e = add_entity<w_entity>();
		e->get_transform()->set_pos( { -112.0f, 0.0f } );
		{
			mesh_mario = e->add_component<ec_mesh>();
			mesh_mario->init( "mesh_mario_jump" );
			mesh_mario->get_transform()->set_scale( 1.25f );
		}
	}

	{
		auto e = add_entity<w_entity>();
		e->get_transform()->set_pos( { 112.0f, 0.0f } );
		{
			mesh_crate = e->add_component<ec_mesh>();
			mesh_crate->init( "mesh_crate" );
			mesh_crate->get_transform()->set_scale( 2.5f );
		}
	}

	// camera
	{
		auto e = add_camera();
	}

	engine->window->set_mouse_mode( mouse_mode::os );
}

void layer_meshes::draw()
{
	{
		scoped_render_state;

		render_state =
		{
			.color = w_color::dark_teal
		};
		w_render::draw_tiled( a_texture::find( "engine_tile_background_stripe" ), w_rect( -viewport_hw, -viewport_hh, viewport_w, viewport_h ) );
	}

	w_layer::draw();
	w_render::draw_world_axis();
}

void layer_meshes::draw_ui()
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

		w_render::draw_string( "Meshes", { ui_w / 2.0f, 16.0f } );
	}
}

void layer_meshes::update()
{
	w_layer::update();

	mesh_mario->get_transform()->set_angle( *mesh_rotator );
	mesh_crate->get_transform()->set_scale( *mesh_scaler );
}

bool layer_meshes::on_input_motion( const w_input_event* evt )
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
