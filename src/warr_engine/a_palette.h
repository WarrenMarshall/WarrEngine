#pragma once

struct a_palette : i_asset
{
	declare_find_func( a_palette );

	std::vector<w_color> colors;

	virtual ~a_palette() override;

	w_color get_color_from_idx( int idx );

	virtual void clean_up_internals() override;
	virtual bool create_internals() override;
};
