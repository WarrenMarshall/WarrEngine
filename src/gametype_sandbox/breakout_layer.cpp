
#include "app_header.h"

breakout_layer::breakout_layer()
{
	draws_completely_solid = true;
}

void breakout_layer::push()
{
	w_layer::push();

	engine->window->set_mouse_mode( mouse_mode::locked );

	breakout_physics = std::make_unique<w_breakout_physics_responder>();
	engine->box2d_world->SetContactListener( breakout_physics.get() );

	engine->box2d_world->SetGravity( { 0, 0 } );
	ec_b2d_body* ec = nullptr;

	// world geo

	auto world_geo = add_entity<w_entity>();
	world_geo->collision_layer = clayer_world;
	world_geo->collides_with = clayer_ball;

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

	{
		spawn_ball();
	}

	{
		player = add_entity<e_breakout_paddle>();
		player->set_position_deep( { v_window_hw, v_window_h - 24.0f }, true );
	}
}

void breakout_layer::pop()
{
	w_layer::pop();

	engine->box2d_world->SetContactListener( nullptr );
}

void breakout_layer::draw()
{
	w_layer::draw();

	float ypos = 0.0f;
	RENDER
		->begin()
		->push_rgba( w_color::teal, 0.5f )
		->draw_string( engine->pixel_font, "'N' - spawn new ball", w_rect( 12, ypos += 12 ) )
		->draw_string( engine->pixel_font, "'mouse' - move paddle", w_rect( 12, ypos += 12 ) )
		->end();
}

void breakout_layer::spawn_ball()
{
	auto e = add_entity<e_breakout_ball>();

	e->set_position_deep( { v_window_hw, v_window_hh }, false );
}

bool breakout_layer::event_input_motion( const w_input_event* evt )
{
	if( evt->input_id == input_id::mouse )
	{
		w_vec2 new_pos = { player->pos.x + evt->delta.x, v_window_h - 24.0f };
		new_pos.x = std::clamp( new_pos.x, 48.0f, v_window_w - 48.0f );
		player->set_position_deep( new_pos, false );
		return true;
	}

	if( evt->input_id == input_id::controller_left_trigger )
	{
		w_vec2 new_pos = { player->pos.x - evt->delta.x, v_window_h - 24.0f };
		new_pos.x = std::clamp( new_pos.x, 48.0f, v_window_w - 48.0f );
		player->set_position_deep( new_pos, false );
		return true;
	}

	if( evt->input_id == input_id::controller_right_trigger || evt->input_id == input_id::controller_left_stick )
	{
		w_vec2 new_pos = { player->pos.x + evt->delta.x, v_window_h - 24.0f };
		new_pos.x = std::clamp( new_pos.x, 48.0f, v_window_w - 48.0f );
		player->set_position_deep( new_pos, false );
		return true;
	}

	return false;
}

bool breakout_layer::event_input_released( const w_input_event* evt )
{
	if( evt->input_id == input_id::key_n || evt->input_id == input_id::controller_button_y )
	{
		spawn_ball();
		return true;
	}

	return false;
}
