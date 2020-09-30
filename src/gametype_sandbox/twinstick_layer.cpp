
#include "app_header.h"

twinstick_layer::twinstick_layer()
{
	draws_completely_solid = true;
}

void twinstick_layer::push()
{
	w_layer::push();

	engine->window->set_mouse_mode( mouse_mode::locked );

	twinstick_physics = std::make_unique<w_twinstick_physics>();
	engine->box2d_world->SetContactListener( twinstick_physics.get() );

	engine->box2d_world->SetGravity( { 0, 0 } );
	ec_b2d_body* ec = nullptr;

	// world geo

	auto world_geo = add_entity<w_entity>();
	world_geo->collision_layer = clayer_world;
	world_geo->collides_with = clayer_player;
	world_geo->draw_debug_info = true;

	ec = world_geo->add_component<ec_b2d_static>();
	{
		// bounding box for world

		ec->add_fixture_line_loop(
			contact_id::world,
			w_vec2::zero,
			{
				{ 4.0f, 4.0f },
				{ v_window_w - 8.0f, 4.0f },
				{ v_window_w - 8.0f, v_window_h - 8.0f },
				{ 4.0f, v_window_h - 8.0f }
			}
		);

#if 0
		//  random shapes

		for( int x = 0 ; x < 10 ; ++x )
		{
			float xpos = w_random::getf_range( 0.0f, v_window_w );
			float ypos = w_random::getf_range( 0.0f, v_window_h - 64 );

			if( w_random::geti_range( 0, 3 ) == 1 )
			{
				float sz = w_random::getf_range( 4, 16 );
				ec->add_fixture_circle( contact_id::world, { xpos, ypos }, sz );
			}
			else
			{
				float sz = w_random::getf_range( 4, 32 );
				float sz2 = w_random::getf_range( 4, 32 );
				ec->add_fixture_box( contact_id::world, w_rect( xpos, ypos, sz, sz2 ) );
			}
		}
#endif
	}

	// player
	{
		player = add_entity<e_twinstick_player>();
		player->set_position_deep( { v_window_hw, v_window_hh }, true );
	}
}

void twinstick_layer::pop()
{
	w_layer::pop();

	engine->box2d_world->SetContactListener( nullptr );
}

void twinstick_layer::update()
{
	w_layer::update();

	twinstick_physics->move_player( player );
	twinstick_physics->update();
}

void twinstick_layer::draw()
{
	w_layer::draw();

	float ypos = 0.0f;
	RENDER
		->begin()
		->push_rgba( w_color::teal, 0.5f )
		->draw_string( engine->pixel_font, "TwinStick Shooter!", w_rect( 12, ypos += 12 ) )
		->end();
}

bool twinstick_layer::handle_input_event( const w_input_event* evt )
{
	if( evt->event_id == event_id::input_motion )
	{
		if( evt->input_id == input_id::controller_left_stick )
		{
			//log_msg("left stick : {},{}", evt->mouse->)
		}
	}

	return true;
}
