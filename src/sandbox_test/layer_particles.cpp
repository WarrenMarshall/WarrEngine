
#include "app_header.h"

layer_particles::layer_particles()
{
	draws_completely_solid = true;
}

void layer_particles::push()
{
	// camera
	{
		auto e = add_camera();
	}

	// background emitters
	{
		auto e = add_entity<w_entity>();

		e->get_transform()->set_pos( { 0.0f, viewport_hh } );
		{
			auto ec = e->add_component<ec_emitter>();
			ec->init( "em_fire_upwards" );
		}
		{
			auto ec = e->add_component<ec_primitive_shape>();
			ec->init( primitive_shape::filled_rectangle, w_rect( -viewport_hw, -2.0f, viewport_w, 4.0f ) );
			ec->rs_opt.color = w_color::light_green;
			ec->rs_opt.color->a = 0.25f;
		}
		{
			auto ec = e->add_component<ec_emitter>();
			ec->init( "em_stars" );
			ec->get_transform()->set_pos( { 0.0f, -viewport_hh } );
		}
		{
			auto ec = e->add_component<ec_primitive_shape>();
			ec->init( primitive_shape::point, 3.0f );
			ec->rs_opt.color = w_color::light_green;
			ec->get_transform()->set_pos( { 0.0f, -viewport_hh } );
		}
	}

	// torch emitter attached to mouse cursor
	{
		auto e = add_entity<w_entity>();
		e->tag = H( "mouse_torch" );

		{
			auto ec = e->add_component<ec_emitter>();
			ec->init( "em_torch" );
		}
		{
			auto ec = e->add_component<ec_primitive_shape>();
			ec->init( primitive_shape::filled_circle, 10.0f );
			ec->rs_opt.color = w_color::light_green;
			ec->rs_opt.color->a = 0.25f;
		}
	}

	engine->window->set_mouse_mode( mouse_mode::os );
}

void layer_particles::update()
{
	w_layer::update();

}

void layer_particles::draw()
{
	render_state =
	{
		.color = w_color::dark_teal
	};

	w_render::draw_tiled( a_texture::find( "engine_tile_background_stripe" ), w_rect( -viewport_hw, -viewport_hh, viewport_w, viewport_h ) );

	w_layer::draw();

	w_render::draw_world_axis();
}

void layer_particles::draw_ui()
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

		w_render::draw_string( "Particles", { ui_w / 2.0f, 16.0f } );
	}

	// compute where the torch is in UI space and draw a label there

	w_vec2 label_pos = find_entity( H( "mouse_torch" ) )->get_transform()->pos;
	label_pos = w_coord::world_to_ui_pos( label_pos, get_camera() );
	label_pos += w_vec2( 8.0f, 0.0f );

	render_state =
	{
		.align = align::left | align::vcenter,
		.color = w_color::orange
	};

	w_render::draw_string( "My Light In The Dark", label_pos );
}

bool layer_particles::on_input_motion( const w_input_event* evt )
{
	if( evt->input_id == input_id::mouse )
	{
		if( INPUT->get_button_state( input_id::mouse_button_left ) == button_state::held )
		{
			auto wpos = w_coord::window_to_world_pos( evt->mouse_pos, get_camera() );
			find_entity( H( "mouse_torch" ) )->get_transform()->set_pos( wpos );
		}

		// camera control
		if( INPUT->get_button_state( input_id::mouse_button_right ) == button_state::held )
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
