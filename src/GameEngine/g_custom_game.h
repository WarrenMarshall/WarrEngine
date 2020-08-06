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

struct g_custom_game : w_game
{
	e_player* player = nullptr;

	int current_room_idx = 0;
	int current_layer_idx = 0;
	std::array<w_world_room, 10> geometry_layer = {};
	std::array<w_world_room, 10> item_layer = {};

	// edit mode specific
	w_vec2 current_tile = w_vec2( 0.0f, 0.0f );
	int current_tile_idx = 0;
	// edit mode specific

	std::array<w_tile, static_cast<size_t>( 256 )> tile_masters;

	g_custom_game();

	void init() override;
	void new_game() override;
	void update() override;

	w_tile* get_tile( int id );
	void draw_viewport_caption( const std::string_view caption, float ypos );
};
