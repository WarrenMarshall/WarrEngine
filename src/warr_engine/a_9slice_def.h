#pragma once

struct a_9slice_def : i_asset
{
	declare_find_func( a_9slice_def )

	std::array<a_texture*, 9> patches = {};
};

