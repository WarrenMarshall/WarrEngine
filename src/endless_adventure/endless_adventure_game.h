#pragma once

constexpr int AREA_SZ = 9;
constexpr int TILE_SZ = 16;

// ----------------------------------------------------------------------------

struct w_tile
{
	int id = 0;
	e_area_layer area_layer = area_layer::nobrowse;
	a_subtexture* subtex = nullptr;
};

// ----------------------------------------------------------------------------

struct w_area
{
	// a set of AREA_SZ*AREA_SZ tile ids, one for each type of room_layer
	int tile_ids[ area_layer::max ][ AREA_SZ * AREA_SZ ];
};

// ----------------------------------------------------------------------------

struct endless_adventure_game final : w_game
{
	// ----------------------------------------------------------------------------
	// Common
	// ----------------------------------------------------------------------------

	// the tile templates, read from tile_def.txt
	std::array<w_tile, static_cast<size_t>( 256 )> tile_masters;

	// the contents of each area
	w_area areas[ 10 ] = {};

	// ----------------------------------------------------------------------------
	// Editor Mode
	// ----------------------------------------------------------------------------

	// the currently active tile that we are painting with
	int current_tile_idx = 0;

	// the area being edited
	int current_area_idx = 0;

	// ----------------------------------------------------------------------------
	// Game Mode
	// ----------------------------------------------------------------------------

	// ----------------------------------------------------------------------------

	endless_adventure_game();

	void init() final;
	void new_game() final;
	void update() final;

	void load_area_data();
	void save_area_data();

	w_tile* get_tile( int id );
};
