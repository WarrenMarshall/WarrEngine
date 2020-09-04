
#include "app_header.h"

void spawn_ball( layer_gameplay* layer )
{
	auto ball = layer->spawn_entity<e_ball>( { v_window_hw, v_window_hh }, 0.0f, 0.5f + (w_random::getf() * 1.0f) );
	ball->add_component<ec_collider>()->init_as_circle( 12 );
	ball->forces.emplace_back( std::make_unique<w_force>( w_vec2::get_random_unit(), 100.0f + ( w_random::getf() * 100.0f ) ) );
}

layer_gameplay::layer_gameplay()
{
}

void layer_gameplay::push()
{
	walls = spawn_entity<w_entity>();

	walls->add_component<ec_collider>()->init_as_box( w_rect( 0.0f, 0.0f, 1.0f, v_window_h ) );
	walls->add_component<ec_collider>()->init_as_box( w_rect( v_window_w - 1.0f, 0.0f, v_window_w, v_window_h ) );
	walls->add_component<ec_collider>()->init_as_box( w_rect( 0.0f, 0.0f, v_window_w, 1.0f ) );
	//walls->debug_draw_collision = true;
	walls->collision_layer = cl_wall;

	death_zone = spawn_entity<w_entity>();
	death_zone->add_component<ec_collider>()->init_as_box( w_rect( 0.0f, v_window_h - 8, v_window_w, v_window_h + 8 ) );
	death_zone->collision_layer = cl_wall;

	paddle = spawn_entity<e_paddle>( { v_window_hw, v_window_h - 12 }, 0.0f, 0.75f );

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
			if( paddle )
			{
				paddle->pos.x += evt->mouse.delta.x;
				paddle->pos.x = w_clamp( paddle->pos.x, 0.0f, v_window_w );
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
	for( auto& ent : entities )
	{
		if( ent->collides_with > 0 )
		{
			for( auto& ent2 : entities )
			{
				if( ent != ent2 )
				{
					if( ent->collides_with & ent2->collision_layer )
					{
						for( auto& collider : ent->ec.colliders )
						{
							for( auto& collider2 : ent2->ec.colliders )
							{
								if( c2Collided( &collider->get_collider(), NULL, collider->c2type,
												&collider2->get_collider(), NULL, collider2->c2type ) )
								{
									c2Manifold hit;
									c2Collide( &collider->get_collider(), NULL, collider->c2type,
											   &collider2->get_collider(), NULL, collider2->c2type,
											   &hit );

									if( hit.count )
									{
										collider->push_outside( hit );
										ent->collided_with( collider2->parent_entity, hit );
									}
								}
							}
						}
					}
				}
			}
		}

	}
#endif
}
