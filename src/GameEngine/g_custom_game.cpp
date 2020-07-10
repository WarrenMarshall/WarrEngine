
#include "master_pch.h"
#include "master_header.h"

g_custom_game::g_custom_game()
{
	auto file = engine->fs->load_file_into_memory( "game_specific\\tile_def.txt" );
	w_mem_file* mf = file.get();
	std::string file_as_string = std::string( mf->buffer->begin(), mf->buffer->end() );

	w_tokenizer tok( file_as_string, '\n', "" );

	std::string line = tok.get_next_token();

	while( !tok.is_eos() )
	{
		if( line.length() > 0 )
		{
			w_tokenizer line_tok( line, ',', "" );

			int tile_id = w_parser::int_from_str( line_tok.get_next_token() );
			std::string tile_name = line_tok.get_next_token();
			bool tile_show_in_browser = w_parser::bool_from_str( line_tok.get_next_token() );

			tile_masters[ tile_id ].id = tile_id;
			tile_masters[ tile_id ].show_in_browser = tile_show_in_browser;
			tile_masters[ tile_id ].img = engine->get_asset<a_subtexture>( tile_name );
		}

		line = tok.get_next_token();
	}

	// populate the rooms with default tile values

	int debug_tile_id = 0;
	for( auto& iter : rooms )
	{
		for( int x = 0 ; x < ROOM_W * ROOM_H ; ++x )
		{
			iter.tiles[ x ] = debug_tile_id;
		}
		debug_tile_id++;
	}
}

void g_custom_game::new_game()
{
	w_game::new_game();

	player = game->spawn_entity<e_player>( { 0, -100 }, 30.f );
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
	assert( tile->img != nullptr );	// there is no tile definition at the requested index - double check "tile_def.txt"
	
	return tile;
}
