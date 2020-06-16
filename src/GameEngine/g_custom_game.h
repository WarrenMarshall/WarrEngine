#pragma once

enum struct e_tile : int
{
	deep_water = 0,
	shallow_water,
	river_water,
	grass_shrub,
	grass,
	light_trees,
	trees,
	hills,
	mountains,
	dungeon_entrance,
	town,
	fort,
	village,
	castle_left,
	castle_middle,
	castle_right,

	player,

	max
};

struct w_tile
{
	e_tile id;
	std::unique_ptr<w_image> img = nullptr;
};

struct g_custom_game : w_game
{
	a_font* font = nullptr;
	e_player* player = nullptr;

	std::array<w_tile, static_cast<size_t>( e_tile::max )> tile_masters;

	g_custom_game();

	virtual void new_game();
	virtual void update();
	virtual void update_fts();

	w_tile* get_tile( e_tile id );
};
