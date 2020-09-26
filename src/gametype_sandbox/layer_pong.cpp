
#include "app_header.h"

layer_pong::layer_pong()
{
	draws_completely_solid = true;
}

void layer_pong::push()
{
	pong_physics = std::make_unique<w_pong_physics>();
	engine->box2d_world->SetContactListener( pong_physics.get() );

	engine->box2d_world->SetGravity( { 0, 0 } );
	ec_b2d_body* ec = nullptr;

	// world geo

	auto world_geo = add_entity<w_entity>();
	world_geo->collision_layer = clayer_world;
	world_geo->collides_with = clayer_ball;
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

#if 1
		//  random shapes

		for( int x = 0 ; x < 15 ; ++x )
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

	{
		spawn_ball();
	}

	{
		player = add_entity<e_pong_paddle>();
		player->set_position( { v_window_hw, v_window_h - 24.0f } );
	}
}

void layer_pong::pop()
{
	w_layer::pop();

	engine->box2d_world->SetContactListener( nullptr );
}

void layer_pong::update()
{
	w_layer::update();

	pong_physics->update();
}

void layer_pong::draw()
{
	w_layer::draw();

	RENDER
		->begin()
		->push_rgba( w_color::teal, 0.5f )
		->draw_string( engine->pixel_font, "'N' - spawn new ball", w_rect( 12, 12 ) )
		->end();
}

void layer_pong::spawn_ball()
{
	auto e = add_entity<e_pong_ball>();
	e->set_position( { v_window_hw, v_window_hh } );
}

bool layer_pong::handle_input_event( const w_input_event* evt )
{
	if( evt->event_id == event_id::input_motion )
	{
		if( evt->input_id == input_id::mouse )
		{
			player->set_position( { player->pos.x + evt->mouse.delta.x, player->pos.y } );
		}
	}

	if( evt->event_id == event_id::input_pressed )
	{
		if( evt->input_id == input_id::key_n )
		{
			spawn_ball();
		}
	}

	return true;
}
