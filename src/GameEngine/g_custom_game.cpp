
#include "master_pch.h"
#include "master_header.h"

g_custom_game::g_custom_game()
{
	#define define_tile( _id, _rect )\
		tile_masters[ (int)_id ].id = _id;\
		tile_masters[ (int)_id ].img = std::make_unique<w_image>( "tex_ultima_atlas", _rect );

	define_tile( e_tile::deep_water, w_rect( 0, 0, 16, 16 ) );
	define_tile( e_tile::shallow_water, w_rect( 16, 0, 16, 16 ) );
	define_tile( e_tile::river_water, w_rect( 32, 0, 16, 16 ) );
	define_tile( e_tile::grass_shrub, w_rect( 48, 0, 16, 16 ) );
	define_tile( e_tile::grass, w_rect( 64, 0, 16, 16 ) );
	define_tile( e_tile::light_trees, w_rect( 80, 0, 16, 16 ) );
	define_tile( e_tile::trees, w_rect( 96, 0, 16, 16 ) );
	define_tile( e_tile::hills, w_rect( 112, 0, 16, 16 ) );
	define_tile( e_tile::mountains, w_rect( 128, 0, 16, 16 ) );
	define_tile( e_tile::dungeon_entrance, w_rect( 144, 0, 16, 16 ) );
	define_tile( e_tile::town, w_rect( 160, 0, 16, 16 ) );
	define_tile( e_tile::fort, w_rect( 176, 0, 16, 16 ) );
	define_tile( e_tile::village, w_rect( 192, 0, 16, 16 ) );
	define_tile( e_tile::castle_left, w_rect( 208, 0, 16, 16 ) );
	define_tile( e_tile::castle_middle, w_rect( 224, 0, 16, 16 ) );
	define_tile( e_tile::castle_right, w_rect( 240, 0, 16, 16 ) );

	define_tile( e_tile::player, w_rect( 240, 16, 16, 16 ) );

	font = engine->get_asset<a_font>( "medieval_font" );
}

void g_custom_game::new_game()
{
	w_game::new_game();

	player = game->spawn_entity<e_player>( w_vec3::zero, 30.f );
}

void g_custom_game::update()
{
	w_game::update();

}

void g_custom_game::update_fts()
{
	w_game::update_fts();
}

w_tile* g_custom_game::get_tile( e_tile id )
{
	return &( tile_masters[ static_cast<size_t>( id ) ] );
}
