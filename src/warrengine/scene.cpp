
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
	simple_collision.components.clear();

	for( auto& entity : entities )
	{
		scoped_opengl;

		auto tform = entity->get_transform();
		g_engine->render_api.top_matrix->apply_transform( tform->pos, tform->angle, tform->scale );

		entity->pre_update();
		entity->pre_update_components();

		// clear simple collisions and add all simple_collision_components for
		// this entity into the list for later

		entity->pending_collisions.clear();

		auto scc = entity->get_components<simple_collision_component>();
		simple_collision.components.insert(
			simple_collision.components.end(),
			scc.begin(), scc.end()
		);
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
	// update entities and components

	for( auto& entity : entities )
	{
		scoped_opengl;

		auto tform = entity->get_transform();
		g_engine->render_api.top_matrix->apply_transform( tform->pos, tform->angle, tform->scale );

		entity->update();
		entity->update_components();
		entity->update_from_physics();
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

	// all entities are in their desired positions. check for and respond to collisions.

	process_simple_collisions();
}

// fills the simple collision queue with all the collisions that are currently
// taking place.

void scene::process_simple_collisions()
{
	std::set<entity*> entities_that_have_collisions;

	// process all the simple_collision_components against each other, and add
	// any collisions and associated info into the collision queue

	for( auto scc_a : simple_collision.components )
	{
		for( auto scc_b : simple_collision.components )
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

			auto aabb_ws_a = scc_a->as_c2_aabb();
			auto aabb_ws_b = scc_b->as_c2_aabb();

			c2Circle circle_a = scc_a->as_c2_circle();
			c2Circle circle_b = scc_b->as_c2_circle();

			bool a_is_circle = scc_a->type == simple_collision_type::circle;
			bool b_is_circle = scc_b->type == simple_collision_type::circle;

			c2Manifold m = {};

			if( a_is_circle and b_is_circle )
			{
				// circle to circle

				if( !c2CircletoCircle( circle_a, circle_b ) )
				{
					continue;
				}

				c2CircletoCircleManifold( circle_a, circle_b, &m );
			}
			else if( !a_is_circle and b_is_circle )
			{
				// aabb to circle

				if( !c2CircletoAABB( circle_b, aabb_ws_a ) )
				{
					continue;
				}

				c2CircletoAABBManifold( circle_b, aabb_ws_a, &m );
				m.n.x *= -1.f;
				m.n.y *= -1.f;
			}
			else if( a_is_circle and !b_is_circle )
			{
				// circle to aabb

				if( !c2CircletoAABB( circle_a, aabb_ws_b ) )
				{
					continue;
				}

				c2CircletoAABBManifold( circle_a, aabb_ws_b, &m );
			}
			else if( !a_is_circle and !b_is_circle )
			{
				// aabb to aabb

				if( !c2AABBtoAABB( aabb_ws_a, aabb_ws_b ) )
				{
					continue;
				}

				c2AABBtoAABBManifold( aabb_ws_a, aabb_ws_b, &m );
			}

			// add the collision to the pending collision list
			simple_collision::pending_collision collision;

			collision.entity_a = scc_a->parent_entity;
			collision.entity_b = scc_b->parent_entity;
			collision.manifold = m;

			collision.closest_point = { m.contact_points[ 0 ].x, m.contact_points[ 0 ].y };
			collision.normal = vec2( m.n.x * -1.f, m.n.y * -1.f );
			collision.depth = m.depths[ 0 ];

			collision.entity_a->pending_collisions.push_back( collision );
			entities_that_have_collisions.insert( collision.entity_a );
		}
	}

	for( auto& e : entities_that_have_collisions )
	{
		e->process_simple_collisions();
	}
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
