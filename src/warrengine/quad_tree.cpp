
#include "master_pch.h"
#include "master_header.h"

namespace war
{

// ----------------------------------------------------------------------------

Quad_Tree::Node::Node( const Rect& rc )
{
	init( rc );
}

void Quad_Tree::Node::init( const Rect& rc )
{
	is_alive = true;
	bounds = rc;
}

void Quad_Tree::Node::debug_draw() const
{
	scoped_render_state;

	Render::state->color = make_color( Color::white, 0.15f );
	Render::draw_rect( bounds );
}

// ----------------------------------------------------------------------------

Quad_Tree::Quad_Tree()
{
	nodes.init_to_size( Quad_Tree::max_nodes_in_pool );
	reset();
}


void Quad_Tree::set_bounds( const Rect& bounds )
{
	this->bounds = bounds;
	nodes.init_to_size( Quad_Tree::max_nodes_in_pool );

	reset();
}

void Quad_Tree::set_bounds_to_viewport()
{
	set_bounds( Rect( 0.f, 0.f, viewport_w, viewport_h ) );
}

void Quad_Tree::reset()
{
	// sanity checking these values
	assert( max_entities_per_node > 1 );
	assert( min_node_area.x > 16 and min_node_area.y > 16 );

	for( auto& node : nodes.objects )
	{
		node.is_alive = false;
	}

	//nodes.init_to_size( Quad_Tree::max_nodes_in_pool );
	nodes.reset();

	auto node = nodes.get_next();
	*node = { bounds };
}

void Quad_Tree::debug_draw() const
{
#ifndef _RELEASE
	if( !g_engine->render.debug.draw_spatial )
	{
		return;
	}

	for( const auto& node : nodes.objects )
	{
		if( !node.is_alive )
		{
			continue;
		}

		node.debug_draw();
	}
#endif
}

void Quad_Tree::set_max_nodes_in_pool( i32 value )
{
	Quad_Tree::max_nodes_in_pool = value;
	nodes.init_to_size( value );
	nodes.reset();
}

// looks at all the nodes and returns a list of nodes that the specified entity
// is touching, using a computed AABB as the bounds checker.

std::set<Quad_Tree::Node*> Quad_Tree::get_nodes_entity_is_touching( Entity* e ) const
{
	std::set<Quad_Tree::Node*> touching_nodes;

	c2AABB entity_ws_aabb = e->simple_collision_ws_aabb.as_c2AABB();

	for( auto& node : nodes.objects )
	{
		if( !node.is_alive )
		{
			continue;
		}

		if( c2AABBtoAABB( entity_ws_aabb, node.bounds.as_c2AABB() ) )
		{
			touching_nodes.insert( ( Quad_Tree::Node* )&node );
		}
	}

	return touching_nodes;
}

std::set<Quad_Tree::Node*> Quad_Tree::get_nodes_circle_is_touching( const Vec2& pos, f32 radius ) const
{
	std::set<Quad_Tree::Node*> touching_nodes;

	c2Circle ws_circle = { pos.as_c2v(), radius };

	for( auto& node : nodes.objects )
	{
		if( !node.is_alive )
		{
			continue;
		}

		if( c2CircletoAABB( ws_circle, node.bounds.as_c2AABB() ) )
		{
			touching_nodes.insert( ( Quad_Tree::Node* )&node );
		}
	}

	return touching_nodes;
}

std::set<Quad_Tree::Node*> Quad_Tree::get_nodes_rect_is_touching( const Rect& rc_aabb ) const
{
	std::set<Quad_Tree::Node*> touching_nodes;

	c2AABB ws_aabb = rc_aabb.as_c2AABB();

	for( auto& node : nodes.objects )
	{
		if( !node.is_alive )
		{
			continue;
		}

		if( c2AABBtoAABB( ws_aabb, node.bounds.as_c2AABB() ) )
		{
			touching_nodes.insert( ( Quad_Tree::Node* )&node );
		}
	}

	return touching_nodes;
}

// take a position and a radius, and returns the colliding set based on which
// nodes that circle touches

std::set<Entity*> Quad_Tree::find_potentially_colliding_entities( const Vec2& pos, f32 radius ) const
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

// looks through the tree, and returns a set of all entities that could
// potentially be interacting with the specified entity

std::set<Entity*> Quad_Tree::find_potentially_colliding_entities( Entity* e ) const
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

std::set<Entity*> Quad_Tree::find_potentially_colliding_entities( const Rect& rc_aabb ) const
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
	bool did_split_a_node = true;

	while( did_split_a_node )
	{
		did_split_a_node = false;

		for( auto& node : nodes.objects )
		{
			if( !node.is_alive
				or node.entities.size() <= max_entities_per_node
				or node.bounds.area() <= ( min_node_area.x * min_node_area.y ) )
			{
				continue;
			}

			did_split_a_node = true;

			// save the list of entities touching the node we are subdividing.
			// they will be reinserted later.

			std::set<Entity*> saved_entities = node.entities;

			// remove the current_node pointer from the nodes list inside the saved
			// entities.

			for( auto& se : saved_entities )
			{
				se->nodes.erase( ( Quad_Tree::Node * )&node );
			}

			// subdivide current_node into 4 quadrants
			auto rcs = node.bounds.subdivide();
			std::vector<Quad_Tree::Node*> new_nodes;
			new_nodes.reserve( 4 );

			node.is_alive = false;

			// add the 4 new nodes into the node list
			for( auto& rc : rcs )
			{
				auto new_node = nodes.get_next();
				*new_node = { rc };

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
	}
}

void Quad_Tree::pre_update()
{
	reset();
}

void Quad_Tree::update()
{
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
