
#include "master_pch.h"
#include "master_header.h"

g_custom_game::g_custom_game()
{
}

void g_custom_game::init()
{
	auto file = engine->fs->load_text_file_into_memory( "game_specific\\tile_def.txt" );

	for( const auto& line : *(file.get()->lines.get()) )
	{
		w_tokenizer line_tok( line, ',', "" );

		int tile_id = w_parser::int_from_str( line_tok.get_next_token() );
		std::string_view tile_name = line_tok.get_next_token();
		bool tile_show_in_browser = w_parser::bool_from_str( line_tok.get_next_token() );

		tile_masters[ tile_id ].id = tile_id;
		tile_masters[ tile_id ].show_in_browser = tile_show_in_browser;
		tile_masters[ tile_id ].subtex = engine->get_asset<a_subtexture>( tile_name );
	}

	// populate the rooms with default tile values

	int debug_tile_id = 0;
	for( auto& iter : geometry_layer )
	{
		for( int x = 0 ; x < ROOM_SZ * ROOM_SZ ; ++x )
		{
			iter.tiles[ x ] = debug_tile_id;
		}
		debug_tile_id++;
	}
}

void g_custom_game::new_game()
{
	w_game::new_game();

	//player = game->spawn_entity<e_player>( { 152, 188 }, 0.f );
}

void g_custom_game::update()
{
	w_game::update();

}

w_tile* g_custom_game::get_tile( int id )
{
	assert( id > -1 );
	assert( id < 256 );

	w_tile* tile = &( tile_masters[ id ] );
	assert( tile->subtex != nullptr );	// there is no tile definition at the requested index - double check "tile_def.txt"
	
	return tile;
}

void g_custom_game::draw_viewport_caption( const std::string_view caption, float ypos)
{
	RENDER
		->begin()
		->push_depth_nudge( 20 )
		->push_rgb( w_color::orange )
		->push_align( align::centered )
		->draw_string( UI->theme->large_font, caption, w_rect( v_window_hw, ypos ) )
		->end();
}
