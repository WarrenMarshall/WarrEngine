
#include "master_pch.h"

namespace war
{

Scene::Scene()
{
	flags.blocks_further_drawing = false;
	flags.blocks_further_input = false;
	flags.is_debug_physics_scene = false;
	flags.blocks_further_update = false;
}

// ----------------------------------------------------------------------------

void Scene::select_by_pick_id( i32 pick_id )
{
	if( !pick_id )
	{
		return;
	}

	for( const auto& e : entities )
	{
		if( e->pick_id == pick_id )
		{
			selected_entities.insert( e.get() );
		}
	}
}

void Scene::deselect_all()
{
	selected_entities.clear();
}

std::vector<Entity*> Scene::get_selected()
{
	std::vector<Entity*> selections;

	for( const auto& e : selected_entities )
	{
		selections.push_back( e );
	}

	return selections;
}

void Scene::pushed()
{
	sc_world = Collision_World( this );
	spatial_map.parent_scene = this;
}

void Scene::popped()
{
}

void Scene::becoming_top_scene()
{
}

void Scene::getting_covered()
{
}

// ----------------------------------------------------------------------------

void Scene::pre_update()
{
	// remove dead entities

	for( auto iter = entities.begin(); iter != entities.end(); iter++ )
	{
		if( iter->get()->can_be_deleted() )
		{
			// since this entity is going away, remove from any entities it may
			// have been stuck to
			for( auto& e : entities )
			{
				e->sticky_set.erase( iter->get() );
			}

			// we remove a single dead entity each update to amortize the cost
			std::erase( entities, *iter );

			break;
		}
	}

	// living entities

	for( auto& entity : entities )
	{
		scoped_opengl;
		{
			// apply the entity transform. we do this here because the
			// components will need this to be applied as well.

			auto tform = entity->get_transform();
			g_engine->opengl_mgr.top_matrix->apply_transform( tform->pos, tform->angle, tform->scale );

			entity->pre_update();
			entity->pre_update_components();
		}
	}

	spatial_map.pre_update();
}

void Scene::update()
{
	sc_world.active_bodies.clear();

	// living entities

	for( auto& entity : entities )
	{
		scoped_opengl;
		{
			// apply the entity transform. we do this here because the
			// components will need this to be applied as well.

			auto tform = entity->get_transform();
			g_engine->opengl_mgr.top_matrix->apply_transform( tform->pos, tform->angle, tform->scale );

			entity->update();
			entity->update_components();
			entity->update_transform_to_match_box2d_components();

			// collect the simple collision bodies active in the scene
			auto sccs = entity->get_components( e_component_type::collision_body );
			sc_world.active_bodies.insert(
				sc_world.active_bodies.end(),
				sccs.begin(), sccs.end()
			);
		}
	}

	spatial_map.update();
}

void Scene::post_update()
{
	sc_world.init_sensor_sets_for_new_frame();

	// simple collision detection

	for( auto iter_counter = 0; iter_counter < sc_world.settings.max_pos_iterations; ++iter_counter )
	{
		// make sure the collision bodies are in the correct world space
		// position, relative to their parent entities

		for( auto& scc : sc_world.active_bodies )
		{
			( ( Collision_Body_Component* )scc )->update_to_match_parent_transform();
		}

		// detect and handle intersecting colliders

		sc_world.need_another_iteration = false;
		sc_world.handle_collisions();

		// If nothing is intersecting anymore, we can skip the remaining iterations

		if( !sc_world.need_another_iteration )
		{
			break;
		}
	}

	sc_world.process_sensor_sets();

	// update entities and components, after physics have run

	for( auto& entity : entities )
	{
		scoped_opengl;
		{
			auto tform = entity->get_transform();
			g_engine->opengl_mgr.top_matrix->apply_transform( tform->pos, tform->angle, tform->scale );

			entity->post_update();
			entity->post_update_components();
		}
	}

	spatial_map.post_update();
}

// ----------------------------------------------------------------------------

void Scene::draw()
{
	for( const auto& entity : entities )
	{
		if( entity->life_cycle.is_dead() )
		{
			continue;
		}

#ifndef _RELEASE
		g_engine->stats.inc( g_engine->stats._entities );
#endif

		{
			scoped_opengl;

			auto tform = entity->get_transform();
			g_engine->opengl_mgr.top_matrix->apply_transform( tform->pos, tform->angle, tform->scale );

			entity->draw();
		}
	}

	spatial_map.debug_draw();
}

void Scene::draw_ui()
{
}

bool Scene::is_topmost_scene() const
{
	return ( g_engine->scene_mgr.get_top() == this );
}

Entity* Scene::find_entity( hash tag )
{
	Entity* e = nullptr;

	// look for an entity with a matching tag
	auto iter = std::find_if(
		entities.begin(),
		entities.end(),
		[&] ( std::unique_ptr<Entity>& e )
		{
			return e->tag == tag;
		}
	);

	if( iter != entities.end() )
	{
		e = ( *iter ).get();
	}

	return e;
}

Entity* Scene::find_entity_by_pick_id( i32 pick_id )
{
	if( pick_id )
	{
		for( const auto& e : entities )
		{
			if( e->pick_id == pick_id )
			{
				return e.get();
			}
		}
	}

	return nullptr;
}

void Scene::new_game()
{
	entities.clear();
}

UI_Callback* Scene::get_ui_callback()
{
	return ( ui_callback ) ? ui_callback : &( g_ui->default_callback );
}

bool Scene::on_input( const Input_Event* evt )
{
	if( g_engine->scene_mgr.get_top() == this )
	{
		if( get_ui_callback()->on_input( evt ) )
		{
			return true;
		}

		if( evt->is_pressed() )
		{
			g_ui->focused.tag = hash_none;
		}
	}

	return false;
}

bool Scene::on_entity_and_sensor_touching_begin( Entity* entity, Collision_Body_Component* sensor )
{
	return false;
}

bool Scene::on_entity_and_sensor_touching( Entity* entity, Collision_Body_Component* sensor )
{
	return false;
}

bool Scene::on_entity_and_sensor_touching_end( Entity* entity, Collision_Body_Component* sensor )
{
	return false;
}

bool Scene::on_entity_collided_with_entity( Entity* entity_a, Entity* entity_b, collision::Pending_Collision& coll )
{
	return false;
}

Vec2 Scene::get_viewport_pivot()
{
	return viewport_pivot;
}

// call to clear any expanded UI controls the next frame. this allows the
// closing of things like dropdown controls without needing to actually click on
// them directly.

void Scene::force_close_expanded_controls()
{
	ui_expanded_tag_begin = ui_expanded_tag_end = hash_none;
	flags.clear_expanded_tag_this_frame = true;
}

void Scene::follow_cam( const Transform* follow_target )
{
	auto current_cam = get_transform()->pos;
	auto desired_cam = -follow_target->pos;
	auto dist = ( current_cam - desired_cam ).get_size();

	// speed is computed as a function of how far away the camera is from the
	// follow transforms position. the further away it is, the faster it moves.
	auto speed = dist / 8.f;

	// stops the endless creep of the slow moving camera. follow cuts off below
	// a certain threshold.
	if( speed < 0.25f )
	{
		return;
	}

	auto lerp_factor = fixed_time_step::per_second( speed );

	current_cam = lerp( current_cam, desired_cam, lerp_factor );

	get_transform()->set_pos( current_cam );
}

}
