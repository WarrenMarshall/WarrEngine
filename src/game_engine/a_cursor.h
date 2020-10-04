#pragma once

struct a_cursor : i_asset
{
	w_vec2 hotspot_offset = w_vec2( 0, 0 );
	a_subtexture* subtex = nullptr;
};

