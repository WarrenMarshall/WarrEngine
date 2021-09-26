
#include "master_pch.h"
#include "master_header.h"

namespace war
{

// ----------------------------------------------------------------------------

void Quad_Tree::Node::debug_draw() const
{
	Render::draw_rect( bounds );

	for( const auto& node : children  )
	{
		node.debug_draw();
	}
}

// ----------------------------------------------------------------------------

void Quad_Tree::set_bounds( const Rect& bounds )
{
	head.bounds = bounds;
	head.children.clear();
}

void Quad_Tree::debug_draw() const
{
	scoped_render_state;

	Render::state->color = make_color( Color::teal );
	head.debug_draw();
}

}
