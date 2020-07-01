#pragma once

constexpr int ROOM_W = 19;
constexpr int ROOM_H = 9;
constexpr int TILE_SZ = 16;

// ----------------------------------------------------------------------------

struct w_tile
{
	int id = 0;
	bool show_in_browser = true;
	a_image* img = nullptr;
};

// ----------------------------------------------------------------------------

struct w_world_room
{
	std::array< int, ROOM_W* ROOM_H> tiles;
};

// ----------------------------------------------------------------------------

struct g_custom_game : w_game
{
	a_font* font = nullptr;
	e_player* player = nullptr;

	int current_room = 0;
	std::array<w_world_room, 10> rooms;

	std::array<w_tile, static_cast<size_t>( 256 )> tile_masters;

	g_custom_game();

	virtual void new_game();
	virtual void update();

	w_tile* get_tile( int id );
};
