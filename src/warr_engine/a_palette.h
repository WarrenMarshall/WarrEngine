#pragma once

struct a_palette : i_asset
{
	declare_find_func( a_palette );

	std::vector<w_color> colors;

	w_color get_color_from_idx( int idx );
};
