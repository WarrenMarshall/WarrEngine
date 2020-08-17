#pragma once

constexpr int ROOM_SZ = 9;
constexpr int TILE_SZ = 16;

// ----------------------------------------------------------------------------

struct w_tile
{
	int id = 0;
	e_room_layer room_layer = room_layer::nobrowse;
	a_subtexture* subtex = nullptr;
};

// ----------------------------------------------------------------------------

struct w_world_room
{
	// a set of ROOM_SZ*ROOM_SZ tile ids, one for each type of room_layer
	int tile_ids[ room_layer::max ][ ROOM_SZ * ROOM_SZ ];
};

// ----------------------------------------------------------------------------

struct endless_adventure_game final : w_game
{
	// ----------------------------------------------------------------------------
	// Common
	// ----------------------------------------------------------------------------

	// the tile templates, read from tile_def.txt
	std::array<w_tile, static_cast<size_t>( 256 )> tile_masters;

	// the contents of each room
	w_world_room rooms[ 10 ];

	// ----------------------------------------------------------------------------
	// Editor Mode
	// ----------------------------------------------------------------------------

	// the currently active tile that we are painting with
	int current_tile_idx = 0;

	// the room being edited
	int current_room_idx = 0;

	// ----------------------------------------------------------------------------
	// Game Mode
	// ----------------------------------------------------------------------------

	// ----------------------------------------------------------------------------

	endless_adventure_game();

	void init() final;
	void new_game() final;
	void update() final;

	w_tile* get_tile( int id );
	void draw_viewport_caption( const std::string_view caption, float ypos );
};
