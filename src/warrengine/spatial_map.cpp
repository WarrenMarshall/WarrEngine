
#include "master_pch.h"
#include "master_header.h"

namespace war
{

// ----------------------------------------------------------------------------

spatial_map_cell::spatial_map_cell( rect rc_ws )
	: rc_ws( rc_ws )
{

}

// ----------------------------------------------------------------------------

// w, h - the number of cells to allocate
// rc_ws - the pos/size of the spatial map, in world space

void spatial_map::init( rect rc_ws, size_t w, size_t h )
{
	float cell_w = rc_ws.w / w;
	float cell_h = rc_ws.h / h;

	cells.clear();
	cells.reserve( w * h );

	// set up the cells

	for( int y = 0 ; y < h ; ++y )
	{
		for( int x = 0 ; x < w ; ++x )
		{
			cells.emplace_back( rect( rc_ws.x + ( cell_w * x ), rc_ws.y + ( cell_h * y ), cell_w, cell_h ) );
		}
	}
}

void spatial_map::draw()
{
	// map hasn't been initialized, which means it isn't being used
	if( w == 0 or h == 0 )
	{
		return;
	}

	scoped_render_state;

	render::state->color = make_color( color::green );

	for( auto& cell : cells )
	{
		render::draw_rect( cell.rc_ws );
	}
}

}
