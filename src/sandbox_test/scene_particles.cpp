
#include "app_header.h"

using namespace war;

scene_particles::scene_particles()
{
	flags.blocks_further_drawing = true;
}

void scene_particles::pushed()
{
	// stars
	{
		auto e = add_entity<entity>();
		e->tag = H( "stars" );
		e->set_pos( { -150.f, -100.f } );

		{
			auto ec = e->add_component<emitter_component>();
			ec->init( "em_stars" );
		}
		{
			auto ec = e->add_component<primitive_shape_component>();
			ec->add_shape( primitive_shape::circle, 30.f );
			ec->rs_opt.color = make_color( pal::brightest );
		}
	}

	// jumping coins
	{
		auto e = add_entity<entity>();
		e->set_pos( { 0.f, viewport_hh } );

		{
			auto ec = e->add_component<emitter_component>();
			ec->init( "em_coin_fountain" );
		}
		{
			auto ec = e->add_component<primitive_shape_component>();
			ec->add_shape( primitive_shape::filled_rect, rect( -viewport_hw, -2.f, viewport_w, 4.f ) );
			ec->rs_opt.color = make_color( 4, 0.25f );
		}
	}

	// torch emitter attached to mouse cursor
	{
		auto e = add_entity<entity>();
		e->set_pos( { 50.f, -100.f } );
		e->tag = H( "mouse_torch" );

		{
			auto ec = e->add_component<emitter_component>();
			ec->init( "em_torch" );
		}
		{
			auto ec = e->add_component<emitter_component>();
			ec->init( "em_torch_embers" );
		}
		{
			auto ec = e->add_component<primitive_shape_component>();
			ec->add_shape( primitive_shape::filled_circle, 10.f );
			ec->rs_opt.color = make_color( pal::brightest );
			ec->rs_opt.color->a = 0.25f;
		}
	}

	g_engine->window.set_mouse_mode( mouse_mode::custom );
}

void scene_particles::draw()
{
	draw_tiled_background();
	scene::draw();
	render::draw_world_axis();
}

void scene_particles::draw_ui()
{
	scene::draw_ui();
	draw_title( "Particles" );

	// compute where the torch is in UI space and draw a label there

	auto viewport_pos = coord_system::world_to_viewport_pos( find_entity( H( "mouse_torch" ) )->get_transform()->pos );
	viewport_pos += { 24.f, 0.f };
	auto ui_pos = coord_system::viewport_to_ui_pos( viewport_pos );

	render::state->align = align::left | align::vcenter;
	render::state->color = make_color( pal::brighter );
	render::draw_string( "My Light In The Dark", ui_pos );
}

bool scene_particles::on_input_motion( const input_event* evt )
{
	if( evt->input_id == input_id::mouse )
	{
		if( g_engine->input.get_button_state( input_id::mouse_button_left ) == button_state::held )
		{
			auto wpos = coord_system::window_to_world_pos( evt->mouse_pos );
			find_entity( H( "mouse_torch" ) )->set_pos( wpos );
		}

		if( g_engine->input.get_button_state( input_id::mouse_button_right ) == button_state::held )
		{
			auto wpos = coord_system::window_to_world_pos( evt->mouse_pos );
			find_entity( H( "stars" ) )->set_pos( wpos );
		}
	}

	return false;
}
