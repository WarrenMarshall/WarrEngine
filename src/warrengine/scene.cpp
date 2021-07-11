
#include "master_pch.h"
#include "master_header.h"

namespace war
{

scene::scene()
{
	flags.blocks_further_drawing = false;
	flags.blocks_further_input = false;
	flags.is_debug_physics_scene = false;
	flags.blocks_further_update = false;
}

// ----------------------------------------------------------------------------

void scene::save_mouse_mode()
{
	saved_mouse_mode = g_engine->window.mouse_mode;
}

void scene::restore_mouse_mode()
{
	// mouse mode has never been saved
	assert( saved_mouse_mode.has_value() );

	g_engine->window.set_mouse_mode( *saved_mouse_mode );
}

// ----------------------------------------------------------------------------

void scene::select_by_pick_id( int pick_id )
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

void scene::deselect_all()
{
	for( const auto& e : entities )
	{
		e->is_selected = false;
	}
}

std::vector<entity*> scene::get_selected()
{
	std::vector<entity*> selections;

	for( const auto& e : entities )
	{
		if( e->is_selected )
		{
			selections.push_back( e.get() );
		}
	}

	return selections;
}

// ----------------------------------------------------------------------------

void scene::pre_update()
{
	for( auto& entity : entities )
	{
		scoped_opengl;

		auto tform = entity->get_transform();
		g_engine->render_api.top_matrix->apply_transform( tform->pos, tform->angle, tform->scale );

		entity->pre_update();
		entity->pre_update_components();
	}

	remove_dead_entities();
}

void scene::remove_dead_entities()
{
	for( auto iter = entities.begin(); iter != entities.end(); iter++ )
	{
		if( iter->get()->can_be_deleted() )
		{
			// we remove a single dead entity each update to amortize the cost
			entities.erase( iter );
			break;
		}
	}
}

void scene::update()
{
	// this list can change between updates, so it needs to be recreated each
	// time. entities get deleted, created, change their collision masks, etc.

	simple_collision.bodies.clear();

	// update entities and components

	for( auto& entity : entities )
	{
		scoped_opengl;

		auto tform = entity->get_transform();
		g_engine->render_api.top_matrix->apply_transform( tform->pos, tform->angle, tform->scale );

		entity->update();
		entity->update_components();
		entity->update_from_physics();

		// collect the simple collision bodies active in the scene
		auto sccs = entity->get_components<simple_collision_body_component>();
		simple_collision.bodies.insert(
			simple_collision.bodies.end(),
			sccs.begin(), sccs.end()
		);
	}
}

void scene::post_update()
{
	// update entities and components, after physics have run

	for( auto& entity : entities )
	{
		scoped_opengl;

		auto tform = entity->get_transform();
		g_engine->render_api.top_matrix->apply_transform( tform->pos, tform->angle, tform->scale );

		entity->post_update();
		entity->post_update_components();
	}

	// loop through the collision
	for( auto iteration_counter = 0 ; iteration_counter < simple_collision_pos_iterations ; ++iteration_counter )
	{
		for( auto& scc : simple_collision.bodies )
		{
			scc->update_to_match_parent_transform();
		}

		// collision detection

		add_simple_collisions_to_pending_queue();

		// If nothing is intersecting anymore, we can abort the rest of the iterations

		if( simple_collision.unique_entities_with_collisions.empty() )
		{
			break;
		}

		// collision resolution

		resolve_pending_simple_collisions();
	}

}

void scene::add_simple_collisions_to_pending_queue()
{
	// process all the simple_collision_components against each other, and add
	// any collisions and associated info into the collision queue

	for( auto scc_a : simple_collision.bodies )
	{
		for( auto scc_b : simple_collision.bodies )
		{
			// simple_collision_components can't collide with themselves
			if( scc_a == scc_b )
			{
				continue;
			}

			// entities can't collide with themselves
			if( scc_a->parent_entity == scc_b->parent_entity )
			{
				continue;
			}

			// if collision masks don't intersect, skip
			if( !( scc_a->collides_with_mask & scc_b->collision_mask ) )
			{
				continue;
			}

			// test the simple_collision_components against each other. if there
			// is an intersection, add the entity and the collision info into
			// the queue.

			simple_collision::pending_collision collision;
			if( scc_a->collides_with( scc_b, collision ) )
			{
				simple_collision.unique_entities_with_collisions.insert( collision.entity_a );
				collision.entity_a->simple_collision.pending_queue.push_back( collision );
			}
		}
	}
}

add sensor to player and see if you can detect the ground
add sensor to playerand see if you can detect the ground
add sensor to playerand see if you can detect the ground
add sensor to playerand see if you can detect the ground
add sensor to playerand see if you can detect the ground

void scene::resolve_pending_simple_collisions()
{
	for( auto& entity : simple_collision.unique_entities_with_collisions )
	{
		vec2 avg_delta = vec2::zero;

		for( auto& pc : entity->simple_collision.pending_queue )
		{
			avg_delta += -pc.normal * pc.depth;

			// when landing on the ground, kill any velocity on the Y axis. this
			// stops it from accruing to the maximum as you run around on flat
			// geo.

			if( glm::abs( pc.normal.y ) > 0.75f )
			{
				entity->velocity.y = 0.0f;
			}
		}

		avg_delta /= (float)entity->simple_collision.pending_queue.size();

		entity->add_delta_pos( avg_delta );
		entity->simple_collision.pending_queue.clear();
	}

	simple_collision.unique_entities_with_collisions.clear();
}

// ----------------------------------------------------------------------------

void scene::draw()
{
	for( const auto& entity : entities )
	{
		g_engine->stats.entities++;

		{
			scoped_opengl;

			auto tform = entity->get_transform();
			g_engine->render_api.top_matrix->apply_transform( tform->pos, tform->angle, tform->scale );

			entity->draw();
		}
	}
}

void scene::draw_ui()
{
}

bool scene::is_topmost_scene() const
{
	return ( g_engine->scenes.get_top() == this );
}

entity* scene::find_entity( hash tag )
{
	entity* e = nullptr;

	// look for an entity with a matching tag
	auto iter = std::find_if(
		entities.begin(),
		entities.end(),
		[&] ( std::unique_ptr<entity>& e )
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

entity* scene::find_entity_by_pick_id( int pick_id )
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

void scene::new_game()
{
	entities.clear();
}

ui_callback* scene::get_ui_callback()
{
	if( ui_callback )
	{
		return ui_callback.get();
	}

	return &( g_ui->default_callback );
}

bool scene::on_input_motion( const input_event* evt )
{
	return false;
}

bool scene::on_input_pressed( const input_event* evt )
{
	if( g_engine->scenes.get_top() == this )
	{

		auto callback = get_ui_callback();
		if( callback->on_input_pressed( evt ) )
		{
			return true;
		}

		g_ui->focused.tag = hash_none;
	}

	return false;
}

bool scene::on_input_held( const input_event* evt )
{
	if( g_engine->scenes.get_top() == this )
	{
		if( get_ui_callback()->on_input_held( evt ) )
		{
			return true;
		}
	}

	return false;
}

bool scene::on_input_released( const input_event* evt )
{
	if( g_engine->scenes.get_top() == this )
	{
		if( get_ui_callback()->on_input_released( evt ) )
		{
			return true;
		}
	}

	return false;
}

bool scene::on_input_key( const input_event* evt )
{
	if( g_engine->scenes.get_top() == this )
	{
		if( get_ui_callback()->on_input_key( evt ) )
		{
			return true;
		}
	}

	return false;
}

transform* scene::get_transform()
{
	return &camera_transform;
}

vec2 scene::get_viewport_pivot()
{
	return viewport_pivot;
}

// call to clear any expanded UI controls the next frame. this allows the
// closing of things like dropdown controls without needing to actually click on
// them directly.

void scene::force_close_expanded_controls()
{
	ui_expanded_tag_begin = ui_expanded_tag_end = hash_none;
	flags.clear_expanded_tag_this_frame = true;
}

}
