#pragma once

constexpr int ROOM_SZ = 9;
constexpr int TILE_SZ = 16;

// ----------------------------------------------------------------------------

struct w_tile
{
	int id = 0;
	bool show_in_browser = true;
	a_subtexture* subtex = nullptr;
};

// ----------------------------------------------------------------------------

struct w_world_room
{
	std::array< int, ROOM_SZ* ROOM_SZ> tiles;
};

// ----------------------------------------------------------------------------

enum_begin( tile_layer )
	geometry = 0,
	items = 1
enum_end

struct endless_adventure_game : w_game
{
	static endless_adventure_game* inst;

	e_player* player = nullptr;

	int current_tile_idx = 0;
	int current_room_idx = 0;
	e_tile_layer tile_layer = tile_layer::geometry;
	std::array<w_world_room, 10> geometry_layer = {};
	std::array<w_world_room, 10> item_layer = {};

	std::array<w_tile, static_cast<size_t>( 256 )> tile_masters;

	endless_adventure_game();

	void init() override;
	void new_game() override;
	void update() override;

	w_tile* get_tile( int id );
	void draw_viewport_caption( const std::string_view caption, float ypos );
};

