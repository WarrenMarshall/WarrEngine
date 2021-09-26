
#include "master_pch.h"
#include "master_header.h"

namespace war
{

// ----------------------------------------------------------------------------

Spatial_Map::Node::Node( const Rect& rc )
	: bounds( rc )
{
}

void Spatial_Map::Node::debug_draw() const
{
	scoped_render_state;

	Render::state->color = make_color( e_pal::darker, 0.25f );

	if( !entities.empty() )
	{
		//Render::draw_filled_rect( bounds );
		//Render::state->color = make_color( e_pal::brighter );
	}

	Render::draw_rect( bounds );
}

// ----------------------------------------------------------------------------

void Spatial_Map::init( const Rect& bounds, int32_t subd_count )
{
	nodes.clear();
	nodes.emplace_back( bounds );

	while( subd_count )
	{
		std::vector<Spatial_Map::Node> old_nodes = nodes;
		nodes.clear();

		for( auto& node : old_nodes )
		{
			auto rcs = node.bounds.subdivide();

			nodes.emplace_back( rcs[ 0 ] );
			nodes.emplace_back( rcs[ 1 ] );
			nodes.emplace_back( rcs[ 2 ] );
			nodes.emplace_back( rcs[ 3 ] );
		}

		subd_count--;
	}
}

void Spatial_Map::debug_draw() const
{
	if( nodes.empty() )
	{
		return;
	}

	scoped_render_state;

	Render::state->color = make_color( Color::teal );

	for( const auto& node : nodes )
	{
		node.debug_draw();
	}
}

// looks at all the nodes and returns a list of nodes that the specified entity
// is touching, using a computed AABB as the bounds checker.

std::vector<Spatial_Map::Node*> Spatial_Map::get_nodes_entity_is_touching( Entity* e )
{
	std::vector<Spatial_Map::Node*> touching_nodes;

	if( nodes.empty() )
	{
		return touching_nodes;
	}

	c2AABB entity_ws_aabb = e->get_ws_bbox().as_rect().as_c2AABB();

	for( auto& node : nodes )
	{
		if( c2AABBtoAABB( entity_ws_aabb, node.bounds.as_c2AABB() ) )
		{
			touching_nodes.push_back( &node );
		}
	}

	return touching_nodes;
}

std::set<Entity*> Spatial_Map::get_potential_entity_colliding_set( Entity* e )
{
	auto touching_nodes = get_nodes_entity_is_touching( e );

	std::set<Entity*> entities;

	for( auto& node : touching_nodes )
	{
		for( auto& ent : node->entities )
		{
			if( ent != e )
			{
				entities.insert( ent );
			}
		}
	}

	return entities;
}

void Spatial_Map::insert_entity( Entity* e )
{
	if( nodes.empty() )
	{
		return;
	}

	auto touching_nodes = get_nodes_entity_is_touching( e );

	for( auto& node : touching_nodes )
	{
		node->entities.insert( e );
	}
}

void Spatial_Map::pre_update()
{
	if( nodes.empty() )
	{
		return;
	}

	// clear all entity pointers from the nodes
	for( auto& node : nodes )
	{
		node.entities.clear();
	}
}

void Spatial_Map::update()
{
	if( nodes.empty() )
	{
		return;
	}

	// insert all needed entities into the nodes

	for( auto& e : parent_scene->entities )
	{
		insert_entity( e.get() );
	}
}

void Spatial_Map::post_update()
{
	if( nodes.empty() )
	{
		return;
	}

}

}
