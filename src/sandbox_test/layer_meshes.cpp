
#include "app_header.h"

// ----------------------------------------------------------------------------

layer_meshes::layer_meshes()
{
	draws_completely_solid = true;

	mesh_rotator = w_tween( 0, 360, 5000, tween_type::pingpong, tween_via::sinusoidal );
}

void layer_meshes::push()
{
	// camera
	{
		auto e = add_camera();
		e->get_transform()->set_pos( { v_window_hw, v_window_hh } );
	}

	{
		auto e = add_entity<w_entity>();
		e->get_transform()->set_pos( { -96.0f, 0.0f } );
		{
			mec = e->add_component<ec_mesh>();
			mec->init( "mesh_mario_jump" );
			mec->get_transform()->set_scale( 1.5f );
		}
	}

	{
		auto e = add_entity<w_entity>();
		e->get_transform()->set_pos( { 96.0f, 0.0f } );
		{
			auto ec = e->add_component<ec_mesh>();
			ec->init( "mesh_crate" );
			ec->get_transform()->set_scale( 3.0f );
		}
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
		w_render::draw_tiled( a_texture::find( "engine_tile_background_stripe" ), w_rect( -v_window_hw, -v_window_hh, v_window_w, v_window_h ) );
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

		w_render::draw_string( "Meshes", { ui_window_w / 2.0f, 16.0f } );

		render_state =
		{
			.align = align::hcenter,
			.color = w_color::light_grey,
			.scale = 1.0f
		};

		w_render::draw_string( "R_DRAG / M_DRAG - move/rotate camera", w_pos( ui_window_hw, 200.0f ) );
	}
}

void layer_meshes::update()
{
	w_layer::update();

	mec->get_transform()->set_angle( *mesh_rotator );
}

bool layer_meshes::on_input_motion( const w_input_event* evt )
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
