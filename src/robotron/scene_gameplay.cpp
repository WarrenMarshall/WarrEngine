
#include "app_header.h"

using namespace war;

Scene_Gameplay::Scene_Gameplay()
{
	flags.blocks_further_drawing = true;
	flags.blocks_further_input = true;
}

void Scene_Gameplay::pushed()
{
	Scene::pushed();

	g_engine->window.push_mouse_mode( e_mouse_mode::locked );

	spatial_map.set_bounds( { -viewport_hw, -viewport_hh, viewport_w, viewport_h } );

	// ----------------------------------------------------------------------------
	// WORLD

	{
		auto e = add_entity<Entity>();
		e->tag = H( "world_geo" );
		e->simple.type = e_physics_body_type::stationary;

		{
			auto ec = e->add_component<Collision_Body_Component>();
			ec->get_transform()->set_pos( { -viewport_hw, viewport_hh - 16.f } );
			ec->set_as_box( viewport_w, 16.f );
			ec->set_collision_flags( coll_flags.world, 0 );
		}
		{
			auto ec = e->add_component<Collision_Body_Component>();
			ec->get_transform()->set_pos( { -viewport_hw, -viewport_hh + 8.f } );
			ec->set_as_box( viewport_w, 16.f );
			ec->set_collision_flags( coll_flags.world, 0 );
		}
		{
			auto ec = e->add_component<Collision_Body_Component>();
			ec->get_transform()->set_pos( { -viewport_hw - 8.f, -viewport_hh } );
			ec->set_as_box( 16.f, viewport_h );
			ec->set_collision_flags( coll_flags.world, 0 );
		}
		{
			auto ec = e->add_component<Collision_Body_Component>();
			ec->get_transform()->set_pos( { viewport_hw - 8.f, -viewport_hh } );
			ec->set_as_box( 16.f, viewport_h );
			ec->set_collision_flags( coll_flags.world, 0 );
		}
		{
			auto ec = e->add_component<Primitive_Shape_Component>();
			Rect rc( -viewport_hw, -viewport_hh, viewport_w, viewport_h );
			rc += Rect( 8.f, 24.f, -16.f, -40.f );
			ec->add_shape( e_primitive_shape::rect, rc );
			ec->rs_opt.color = make_color( Color::orange );
			ec->rs_opt.glow = 3.f;
		}
	}

	// ----------------------------------------------------------------------------
	// PLAYER

	player = add_entity<E_Player>();
}

void Scene_Gameplay::popped()
{
	Scene::popped();

	g_engine->window.pop_mouse_mode();
}

void Scene_Gameplay::draw()
{
	Scene::draw();

}

bool Scene_Gameplay::on_input_pressed( const Input_Event* evt )
{
	if( Scene::on_input_pressed( evt ) )
	{
		return true;
	}

	if( evt->input_id == e_input_id::gamepad_button_a )
	{
		{
			auto e = add_entity<Entity_Transient>();
			{
				auto ec = e->add_component<Emitter_Component>();
				ec->init( "em_death_spark" );
			}
		}

		return true;

	}

	return false;
}

bool Scene_Gameplay::on_input_motion( const Input_Event* evt )
{
	if( Scene::on_input_motion( evt ) )
	{
		return true;
	}

	switch( evt->input_id )
	{
		case e_input_id::gamepad_left_stick:
		{
			Vec2 movement = Vec2::snap_to_int( evt->delta );
			if( !movement.is_zero() )
			{
				movement.normalize();
				player->add_delta_pos( movement );

				if( glm::abs( evt->delta.x ) > glm::abs( evt->delta.y ) )
				{
					if( glm::sign( evt->delta.x ) > 0.f )
					{
						player->sprite_component->init( "anim_player_walk_right" );
					}
					else
					{
						player->sprite_component->init( "anim_player_walk_left" );
					}
				}
				else
				{
					if( glm::sign( evt->delta.y ) < 0.f )
					{
						player->sprite_component->init( "anim_player_walk_up" );
					}
					else
					{
						player->sprite_component->init( "anim_player_walk_down" );
					}
				}
			}
			return true;
		}

		case e_input_id::gamepad_right_stick:
		{
			player->fire_weapon( ( i32 )Vec2::dir_to_angle( evt->delta ) );

			return true;
		}
	}

	return false;
}

bool Scene_Gameplay::on_entity_collided_with_entity( Entity* entity_a, Entity* entity_b, collision::Pending_Collision& coll )
{
	if( entity_a->tag == H( "player_bullet" ) and entity_b->tag == H( "world_geo" ) )
	{
		entity_a->life_cycle.set( e_life_cycle::dead );

		{
			auto e = add_entity<Entity_Transient>();
			e->set_pos( entity_a->get_transform()->pos );
			{
				auto ec = e->add_component<Emitter_Component>();
				ec->init( "em_death_spark" );
			}
		}

		return true;
	}


	return false;
}
