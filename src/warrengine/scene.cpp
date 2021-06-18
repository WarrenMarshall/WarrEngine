
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
			selections.emplace_back( e.get() );
		}
	}

	return selections;
}

// ----------------------------------------------------------------------------

void scene::pre_update()
{
	// update entities and components, before the delete logic runs and before
	// the regular update happens

	for( auto& e : entities )
	{
		{
			scoped_opengl;

			auto tform = e->get_transform();
			g_engine->render_api.top_matrix->apply_transform( tform->pos, tform->angle, tform->scale );

			e->pre_update();
		}
	}

	// look for dead entities and remove them

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
	simple_collision_components.clear();

	// update entities and components

	for( auto& entity : entities )
	{
		{
			scoped_opengl;

			auto tform = entity->get_transform();
			g_engine->render_api.top_matrix->apply_transform( tform->pos, tform->angle, tform->scale );

			entity->update();
			entity->update_components();
			entity->update_from_physics();

			// gather a list of all simple collision components that exist on
			// this entity

			auto scc = entity->get_component<simple_collision_component>();
			if( scc )
			{
				simple_collision_components.emplace_back( scc );
			}
		}
	}
}

void scene::post_update()
{
	// process simple collisions

	for( auto scc_a : simple_collision_components )
	{
		for( auto scc_b : simple_collision_components )
		{
			if( scc_a->parent_entity != scc_b->parent_entity )
			{
				if( scc_a->collides_with_mask & scc_b->collision_mask )
				{
					auto aabb_ws_a = scc_a->aabb_ws.to_c2AABB();
					auto aabb_ws_b = scc_b->aabb_ws.to_c2AABB();

					//transform scale_tform;
					//scale_tform.set_scale( scc_b->parent_entity->get_transform()->scale );
					//scale_tform.multiply_scale( scc_b->get_transform()->scale );
					//auto scale_mtx = scale_tform.to_matrix();

					// quick boolean check
					if( c2AABBtoAABB( aabb_ws_a, aabb_ws_b ) )
					{
						// more complex check
						c2Manifold m;
						c2AABBtoAABBManifold( aabb_ws_a, aabb_ws_b, &m );

						// add the collision to the pending collision list
						simple_collision::pending_collision collision;

						collision.entity_a = scc_a->parent_entity;
						collision.entity_b = scc_b->parent_entity;
						collision.manifold = m;

						collision.closest_point = { m.contact_points[ 0 ].x, m.contact_points[ 0 ].y };
						collision.normal = { m.n.x, m.n.y };
						collision.depth = m.depths[ 0 ];
						//scale_mtx.transform_vec2( &collision.contact_point );

						g_engine->simple_collision.queue.emplace_back( collision );
					}
				}
			}
		}
	}

	// update entities and components, after physics have run

	for( auto& entity : entities )
	{
		{
			scoped_opengl;

			auto tform = entity->get_transform();
			g_engine->render_api.top_matrix->apply_transform( tform->pos, tform->angle, tform->scale );

			entity->post_update();
		}
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
