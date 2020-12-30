#pragma once

struct a_cursor : i_asset
{
	declare_find_func( a_cursor )

	w_vec2 hotspot_offset = w_vec2( 0, 0 );
	a_texture* subtex = nullptr;
};

