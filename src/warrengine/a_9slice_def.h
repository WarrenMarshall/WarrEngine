#pragma once

struct a_9slice_def : i_asset
{
	declare_find_func( a_9slice_def )

	std::array<a_texture*, 9> patches = {};

	float get_left_slice_sz() const;
	float get_right_slice_sz() const;
	float get_top_slice_sz() const;
	float get_bottom_slice_sz() const;
};
