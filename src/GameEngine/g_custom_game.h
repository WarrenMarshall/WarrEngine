#pragma once

constexpr int ROOM_W = 19;
constexpr int ROOM_H = 9;
constexpr int TILE_SZ = 16;

struct w_tile
{
	int id;
	std::string name;
	bool show_in_browser;
	std::unique_ptr<w_image> img = nullptr;
};

struct w_world_room
{
	std::array< int, ROOM_W* ROOM_H> tiles;
};

struct g_custom_game : w_game
{
	a_font* font = nullptr;
	e_player* player = nullptr;

	w_world_room test_room;

	std::array<w_tile, static_cast<size_t>( 256 )> tile_masters;

	g_custom_game();

	virtual void new_game();
	virtual void update();
	virtual void update_fts();

	w_tile* get_tile( int id );
};
