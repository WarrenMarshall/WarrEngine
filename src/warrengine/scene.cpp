
#include "master_pch.h"
#include "master_header.h"

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

void Scene::save_mouse_mode()
{
	saved_mouse_mode = g_engine->window.mouse_mode;
}

void Scene::restore_mouse_mode()
{
	// mouse mode has never been saved
	assert( saved_mouse_mode.has_value() );

	g_engine->window.set_mouse_mode( *saved_mouse_mode );
}

// ----------------------------------------------------------------------------

void Scene::select_by_pick_id( int32_t pick_id )
{
	if( !pick_id )
	{
		return;
	}

	for( const auto& e : entities )
	{
		if( e->pick_id == pick_id )
		{
			e->is_selected = true;
		}
	}
}

void Scene::deselect_all()
{
	for( const auto& e : entities )
	{
		e->is_selected = false;
	}
}

std::vector<Entity*> Scene::get_selected()
{
	std::vector<Entity*> selections;

	for( const auto& e : entities )
	{
		if( e->is_selected )
		{
			selections.push_back( e.get() );
		}
	}

	return selections;
}

void Scene::pushed()
{
	sc_world = Simple_Collision_World( this );
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
	for( auto& entity : entities )
	{
		scoped_opengl;

		auto tform = entity->get_transform();
		g_engine->opengl_mgr.top_matrix->apply_transform( tform->pos, tform->angle, tform->scale );

		entity->pre_update();
		entity->pre_update_components();
	}

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
			entities.erase( iter );

			break;
		}
	}

	spatial_map.pre_update();
}

void Scene::update()
{
	if( !entities.empty() )
	{
		sc_world.active_bodies.clear();

		// update entities and components

		for( auto& entity : entities )
		{
			scoped_opengl;

			auto tform = entity->get_transform();
			g_engine->opengl_mgr.top_matrix->apply_transform( tform->pos, tform->angle, tform->scale );

			entity->update();
			entity->update_components();
			entity->update_from_physics();

			// collect the simple collision bodies active in the scene
			auto sccs = entity->get_components<Simple_Collision_Body>();
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

	for( auto iter_counter = 0 ; iter_counter < sc_world.settings.num_pos_iterations ; ++iter_counter )
	{
		// make sure the collision bodies are in the correct world space
		// position, relative to their parent entities

		for( auto& scc : sc_world.active_bodies )
		{
			scc->update_to_match_parent_transform();
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

		auto tform = entity->get_transform();
		g_engine->opengl_mgr.top_matrix->apply_transform( tform->pos, tform->angle, tform->scale );

		entity->post_update();
		entity->post_update_components();
	}

	spatial_map.post_update();
}

// ----------------------------------------------------------------------------

void Scene::draw()
{
	for( const auto& entity : entities )
	{
		g_engine->stats.entities++;

		{
			scoped_opengl;

			auto tform = entity->get_transform();
			g_engine->opengl_mgr.top_matrix->apply_transform( tform->pos, tform->angle, tform->scale );

			entity->draw();
		}
	}
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

Entity* Scene::find_entity_by_pick_id( int32_t pick_id )
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

bool Scene::on_input_motion( const Input_Event* evt )
{
	return false;
}

bool Scene::on_input_pressed( const Input_Event* evt )
{
	if( g_engine->scene_mgr.get_top() == this )
	{
		if( get_ui_callback()->on_input_pressed( evt ) )
		{
			return true;
		}

		g_ui->focused.tag = hash_none;
	}

	return false;
}

bool Scene::on_input_held( const Input_Event* evt )
{
	if( g_engine->scene_mgr.get_top() == this )
	{
		if( get_ui_callback()->on_input_held( evt ) )
		{
			return true;
		}
	}

	return false;
}

bool Scene::on_input_released( const Input_Event* evt )
{
	if( g_engine->scene_mgr.get_top() == this )
	{
		if( get_ui_callback()->on_input_released( evt ) )
		{
			return true;
		}
	}

	return false;
}

bool Scene::on_input_key( const Input_Event* evt )
{
	if( g_engine->scene_mgr.get_top() == this )
	{
		if( get_ui_callback()->on_input_key( evt ) )
		{
			return true;
		}
	}

	return false;
}

bool Scene::on_entity_and_sensor_touching_begin( Entity* entity, Simple_Collision_Body* sensor )
{
	return false;
}

bool Scene::on_entity_and_sensor_touching( Entity* entity, Simple_Collision_Body* sensor )
{
	return false;
}

bool Scene::on_entity_and_sensor_touching_end( Entity* entity, Simple_Collision_Body* sensor )
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
