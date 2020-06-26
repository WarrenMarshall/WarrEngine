
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

rs_color::rs_color( float r, float g, float b )
	: new_color( w_color( r, g, b ) )
{
	push();
}

rs_color::rs_color( w_color color )
	: new_color( color )
{
	push();
}

rs_color::~rs_color()
{
	pop();
}

void rs_color::push()
{
	RENDER->rs_color_stack.push( new_color );
}

void rs_color::pop()
{
	RENDER->rs_color_stack.pop();
}

// ----------------------------------------------------------------------------
