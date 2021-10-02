
#include "master_pch.h"
#include "master_header.h"

namespace war
{

// ----------------------------------------------------------------------------

Quad_Tree::Node::Node( const Rect& rc )
	: bounds( rc )
{
}

void Quad_Tree::Node::debug_draw() const
{
	scoped_render_state;

	Render::state->z += zdepth_nudge;
	Render::state->color = make_color( Color::dark_grey, 0.5f );
	Render::draw_filled_rect( bounds );
	Render::state->color = make_color( Color::white, 0.15f );
	Render::draw_rect( bounds );
}

// ----------------------------------------------------------------------------

void Quad_Tree::init( const Rect& bounds )
{
	this->bounds = bounds;

	reset();
}

void Quad_Tree::reset()
{
	nodes.clear();

	// break the initial bounds into 4 quadrants. we start with 1 level of
	// subdivision because why would you be using a spatial map if everything is
	// in a single node?
#if 0
	auto rcs = bounds.subdivide();

	nodes.emplace_back( rcs[ 0 ] );
	nodes.emplace_back( rcs[ 1 ] );
	nodes.emplace_back( rcs[ 2 ] );
	nodes.emplace_back( rcs[ 3 ] );
#else
	nodes.push_back( std::make_unique<Quad_Tree::Node>( bounds ) );
#endif
}

void Quad_Tree::debug_draw() const
{
	if( nodes.empty() )
	{
		return;
	}

	for( const auto& node : nodes )
	{
		node->debug_draw();
	}
}

// looks at all the nodes and returns a list of nodes that the specified entity
// is touching, using a computed AABB as the bounds checker.

std::vector<Quad_Tree::Node*> Quad_Tree::get_nodes_entity_is_touching( Entity* e ) const
{
	std::vector<Quad_Tree::Node*> touching_nodes;

	if( nodes.empty() )
	{
		return touching_nodes;
	}

	c2AABB entity_ws_aabb = e->simple_collision_ws_aabb.as_c2AABB();

	for( auto& node : nodes )
	{
		if( c2AABBtoAABB( entity_ws_aabb, node->bounds.as_c2AABB() ) )
		{
			touching_nodes.push_back( node.get() );
		}
	}

	return touching_nodes;
}

std::vector<Quad_Tree::Node*> Quad_Tree::get_nodes_circle_is_touching( const Vec2& pos, float_t radius ) const
{
	std::vector<Quad_Tree::Node*> touching_nodes;

	if( nodes.empty() )
	{
		return touching_nodes;
	}

	c2Circle ws_circle = { pos.as_c2v(), radius };

	for( auto& node : nodes )
	{
		if( c2CircletoAABB( ws_circle, node->bounds.as_c2AABB() ) )
		{
			touching_nodes.push_back( node.get() );
		}
	}

	return touching_nodes;
}

std::vector<war::Quad_Tree::Node*> Quad_Tree::get_nodes_rect_is_touching( const Rect& rc_aabb ) const
{
	std::vector<Quad_Tree::Node*> touching_nodes;

	if( nodes.empty() )
	{
		return touching_nodes;
	}

	c2AABB ws_aabb = rc_aabb.as_c2AABB();

	for( auto& node : nodes )
	{
		if( c2AABBtoAABB( ws_aabb, node->bounds.as_c2AABB() ) )
		{
			touching_nodes.push_back( node.get() );
		}
	}

	return touching_nodes;
}

// take a position and a radius, and returns the colliding set based on which
// nodes that circle touches

std::set<Entity*> Quad_Tree::get_potential_entity_colliding_set( const Vec2& pos, float_t radius ) const
{
	std::set<Entity*> entities;

	auto touching_nodes = get_nodes_circle_is_touching( pos, radius );

	for( auto& node : touching_nodes )
	{
		for( auto& ent : node->entities )
		{
			entities.insert( ent );
		}
	}

	return entities;
}

std::set<Entity*> Quad_Tree::get_potential_entity_colliding_set( Entity* e ) const
{
	std::set<Entity*> entities;

	if( e->flags.include_in_quad_tree )
	{
		// if the entity is in the quad_tree, then we can easily get the
		// colliding set by looking at which nodes it is part of and which
		// entities are included in those nodes.

		for( auto& node : e->nodes )
		{
			for( auto& ent : node->entities )
			{
				if( ent == e )
				{
					continue;
				}

				entities.insert( ent );
			}
		}
	}
	else
	{
		// if entity is NOT included in the quad_tree, then we need to figure
		// out which nodes it's bounding box touches and then use that to figure
		// out the colliding set.

		auto touching_nodes = get_nodes_entity_is_touching( e );

		for( auto& node : touching_nodes )
		{
			for( auto& ent : node->entities )
			{
				if( ent == e )
				{
					continue;
				}

				entities.insert( ent );
			}
		}
	}

	return entities;
}

std::set<Entity*> Quad_Tree::get_potential_entity_colliding_set( const Rect& rc_aabb ) const
{
	std::set<Entity*> entities;

	auto touching_nodes = get_nodes_rect_is_touching( rc_aabb );

	for( auto& node : touching_nodes )
	{
		for( auto& ent : node->entities )
		{
			entities.insert( ent );
		}
	}

	return entities;
}

void Quad_Tree::insert_entity( Entity* e )
{
	if( nodes.empty() )
	{
		return;
	}

	auto touching_nodes = get_nodes_entity_is_touching( e );

	for( auto& node : touching_nodes )
	{
		node->entities.insert( e );
		e->nodes.insert( node );
	}

	subdivide_nodes_as_necessary();
}

// looks at the current set of nodes and splits them if necessary. the main
// condition for this being that there are too many entities inside of them.

void Quad_Tree::subdivide_nodes_as_necessary()
{
	std::list<std::unique_ptr<Quad_Tree::Node>> new_node_list;

	for( auto& node : nodes )
	{
		if( node->entities.size() >= max_entities_per_node and node->bounds.area() >= ( min_node_area.x * min_node_area.y ) )
		{
			// save the list of entities touching the node we are subdividing.
			// they will be reinserted later.

			auto saved_entities = node->entities;

			// remove the current_node pointer from the nodes list inside the saved
			// entities.

			for( auto& se : saved_entities )
			{
				se->nodes.erase( node.get() );
			}

			// subdivide current_node into 4 quadrants
			auto rcs = node->bounds.subdivide();
			std::vector<Quad_Tree::Node*> new_nodes;
			new_nodes.reserve( 4 );

			// add the 4 new nodes into the node list
			for( auto& rc : rcs )
			{
				new_node_list.push_back( std::make_unique<Quad_Tree::Node>( rc ) );

				Quad_Tree::Node* new_node = new_node_list.back().get();

				new_nodes.push_back( new_node );
			}

			// look at each saved entity and place it into whichever of the new
			// nodes it is touching.

			for( auto& se : saved_entities )
			{
				c2AABB entity_ws_aabb = se->simple_collision_ws_aabb.as_c2AABB();

				for( auto& nn : new_nodes )
				{
					c2AABB node_aabb = nn->bounds.as_c2AABB();
					if( c2AABBtoAABB( entity_ws_aabb, node_aabb ) )
					{
						nn->entities.insert( se );
						se->nodes.insert( nn );
					}
				}
			}
		}
		else
		{
			new_node_list.push_back( std::move( node ) );
		}
	}

	nodes.clear();
	for( auto& node : new_node_list )
	{
		nodes.push_back( std::move( node ) );
	}
//	nodes = new_node_list;
}

void Quad_Tree::pre_update()
{
	if( nodes.empty() )
	{
		return;
	}

	reset();
}

void Quad_Tree::update()
{
	if( nodes.empty() )
	{
		return;
	}

	// insert all needed entities into the nodes

	for( auto& e : parent_scene->entities )
	{
		if( e->flags.include_in_quad_tree )
		{
			insert_entity( e.get() );
		}
	}
}

void Quad_Tree::post_update()
{
}

}
