#pragma once

// this class exists because cursors need to know where their hot spot is.

struct a_cursor : i_asset
{
	declare_find_func( a_cursor )

	w_vec2 hotspot_offset = w_vec2( 0, 0 );
	a_texture* texture = nullptr;
};
