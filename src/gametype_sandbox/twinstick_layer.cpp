
#include "app_header.h"

twinstick_layer::twinstick_layer()
{
	draws_completely_solid = true;
	music = engine->get_asset<a_sound>( "twinstick_music_track" );
}

void twinstick_layer::push()
{
	w_layer::push();

	//music->play();

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
			"world",
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

		for( int x = 0 ; x < 10 ; ++x )
		{
			float xpos = w_random::getf_range( 0.0f, v_window_w );
			float ypos = w_random::getf_range( 0.0f, v_window_h - 64 );

			if( w_random::geti_range( 0, 3 ) == 1 )
			{
				float sz = w_random::getf_range( 4, 16 );
				ec->add_fixture_circle( "world", { xpos, ypos }, sz );
			}
			else
			{
				float sz = w_random::getf_range( 4, 32 );
				float sz2 = w_random::getf_range( 4, 32 );
				ec->add_fixture_box( "world", w_rect( xpos, ypos, sz, sz2 ) );
			}
		}
#endif
	}

	// player
	{
		player = add_entity<e_twinstick_player>();
		player->set_position_deep( { v_window_hw, v_window_hh }, true );
	}

	// camera

	auto player_camera = add_entity<w_camera>();
	player_camera->attach_to( player );
	RENDER->current_camera = player_camera;
}

void twinstick_layer::pop()
{
	RENDER->current_camera = nullptr;
	//music->stop();

	engine->box2d_world->SetContactListener( nullptr );

	w_layer::pop();
}

void twinstick_layer::update()
{
	w_layer::update();

	twinstick_physics->handle_user_input( player );
	twinstick_physics->update();
}

void twinstick_layer::draw()
{
	w_layer::draw();

	float ypos = 0.0f;
	RENDER
		->begin()
		->push_rgba( w_color::teal, 0.5f )
		->draw_string( engine->pixel_font, "TwinStick Shooter!", w_rect( 12, ypos += 12 ) );

	if( twinstick_physics->trace_hit_location != w_vec2::zero )
	{
		RENDER
			->push_rgb( w_color::red )
			->push_alpha( 0.15f )
			->draw_line( player->pos, twinstick_physics->trace_hit_location );
	}

	RENDER
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
