#pragma once

constexpr int ROOM_W = 19;
constexpr int ROOM_H = 9;
constexpr int TILE_SZ = 16;

// ----------------------------------------------------------------------------

struct w_tile
{
	int id = 0;
	bool show_in_browser = true;
	a_subtexture* subtexture = nullptr;
};

// ----------------------------------------------------------------------------

struct w_world_room
{
	std::array< int, ROOM_W* ROOM_H> tiles;
};

// ----------------------------------------------------------------------------

struct g_custom_game : w_game
{
	e_player* player = nullptr;

	int current_room = 0;
	std::array<w_world_room, 10> rooms = {};

	// edit mode specific
	std::string viewport_caption = "";
	w_vec2 current_tile = w_vec2( 0.0f, 0.0f );
	int current_tile_idx = 0;
	// edit mode specific

	std::array<w_tile, static_cast<size_t>( 256 )> tile_masters;

	g_custom_game();

	void new_game() override;
	void update() override;

	w_tile* get_tile( int id );
	void draw_viewport_caption();
};
