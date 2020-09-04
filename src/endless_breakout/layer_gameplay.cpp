
#include "app_header.h"

void spawn_ball( layer_gameplay* layer )
{
	auto ball = layer->spawn_entity<e_ball>( { v_window_hw, v_window_hh }, 0.0f, 0.5f + (w_random::getf() * 1.0f) );
	layer->balls.push_back( ball );
	ball->add_component<ec_collider>()->init_as_circle( 12 );
	ball->forces.emplace_back( std::make_unique<w_force>( w_vec2::get_random_unit(), 100.0f + ( w_random::getf() * 100.0f ) ) );
}

layer_gameplay::layer_gameplay()
{
}

void layer_gameplay::push()
{
	float thiccness = 1.0f;

	walls = spawn_entity<w_entity>();
	walls->add_component<ec_collider>()->init_as_box( w_rect( 0.0f, 0.0f, thiccness, v_window_h ) );
	walls->add_component<ec_collider>()->init_as_box( w_rect( v_window_w - thiccness, 0.0f, v_window_w, v_window_h ) );
	walls->add_component<ec_collider>()->init_as_box( w_rect( 0.0f, 0.0f, v_window_w, thiccness ) );
	walls->add_component<ec_collider>()->init_as_box( w_rect( 0.0f, v_window_h - thiccness, v_window_w, v_window_h ) );

	walls->add_component<ec_collider>()->init_as_box( w_rect( 64,64,128,24 ) );
	walls->add_component<ec_collider>()->init_as_box( w_rect( 200, 164, 8, 128 ) );
	walls->debug_draw_collision = true;

	//death_zone = spawn_entity<w_entity>();
	//death_zone->add_component<ec_collider>()->init_as_box( w_rect( 0.0f, v_window_h - 8, v_window_w, v_window_h + 8 ) );

	player = spawn_entity<e_player>( { v_window_hw, v_window_h - 12 }, 0.0f, 0.75f );

	for( int x = 0 ; x < 1 ; ++x )
		spawn_ball( this );

	engine->window->set_mouse_mode( mouse_mode::locked );
}

bool layer_gameplay::handle_input_event( const w_input_event* evt )
{
	if( evt->event_id == event_id::input_motion )
	{
		if( evt->input_id == input_id::mouse )
		{
			if( player )
			{
				player->pos.x += evt->mouse.delta.x;
				player->pos.x = w_clamp( player->pos.x, 0.0f, v_window_w );
			}
			return true;
		}
	}

	if( evt->event_id == event_id::input_pressed )
	{
		if( evt->input_id == input_id::key_n )
		{
			spawn_ball( this );
		}
	}

	return false;
}

void layer_gameplay::update_collisions()
{
	w_layer::update_collisions();

#if 1
	for( auto& ball : balls )
	{
		for( auto& ball_collider : ball->ec.colliders )
		{
			for( auto& wall_collider : walls->ec.colliders )
			{
				c2Manifold hit;
				c2Collide( &ball_collider->get_collider(), NULL, ball_collider->c2type,
						   &wall_collider->get_collider(), NULL, wall_collider->c2type,
						   &hit );

				if( hit.count )
				{
					ball_collider->push_outside( hit );
					ball->collided_with( wall_collider->parent_entity, hit );
				}
			}
		}
	}

	for( auto& ball : balls )
	{
		for( auto& ball_collider : ball->ec.colliders )
		{
			for( auto& ball2 : balls )
			{
				if( ball != ball2 )
				{
					for( auto& ball2_collider : ball2->ec.colliders )
					{
						c2Manifold hit;
						c2Collide( &ball_collider->get_collider(), NULL, ball_collider->c2type,
								   &ball2_collider->get_collider(), NULL, ball2_collider->c2type,
								   &hit );

						if( hit.count )
						{
							ball_collider->push_outside( hit );
							ball->collided_with( ball2, hit );
							ball2->collided_with( ball, hit );
						}
					}
				}
			}
		}
	}
#endif
}
