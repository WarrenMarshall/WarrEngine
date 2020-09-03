
#include "app_header.h"

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

	//death_zone = spawn_entity<w_entity>();
	//death_zone->add_component<ec_collider>()->init_as_box( w_rect( 0.0f, v_window_h - 8, v_window_w, v_window_h + 8 ) );

	//player = spawn_entity<e_player>( { v_window_hw, v_window_h - 12 }, 0.0f, 0.75f );

	auto ball = spawn_entity<e_ball>( { v_window_hw, v_window_hh }, 0.0f, 0.75f );
	balls.push_back( ball );
	ball->add_component<ec_collider>()->init_as_circle( 8 );
	ball->forces.emplace_back( std::make_unique<w_force>( w_vec2::get_random_unit(), 150.0f ) );
	ball->debug = true;

	engine->window->set_mouse_mode( mouse_mode::locked );
}

bool layer_gameplay::handle_input_event( const w_input_event* evt )
{
	if( evt->event_id == event_id::input_motion )
	{
		if( evt->input_id == input_id::mouse )
		{
			//player->pos.x += evt->mouse.delta.x;
			//player->pos.x = w_clamp( player->pos.x, 0.0f, v_window_w );
			return true;
		}
	}

	if( evt->event_id == event_id::input_pressed )
	{
		if( evt->input_id == input_id::key_n )
		{
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
				c2Manifold manifold;
				auto coll_a = std::get<c2Circle>( ball_collider->get_collider() );
				auto coll_b = std::get<c2AABB>( wall_collider->get_collider() );
				c2Collide( &coll_a, NULL, ball_collider->c2type,
						   &coll_b, NULL, wall_collider->c2type,
						   &manifold );

				if( manifold.count )
				{
					ball->collided_with( wall_collider->parent_entity, manifold );
				}
			}
		}
	}
#endif
}
