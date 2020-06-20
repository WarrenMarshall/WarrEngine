
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
		line = w_stringutil::trim( line );

		if( line.length() > 0 )
		{
			w_tokenizer line_tok( line, ',', "" );

			int tile_id = w_parser::parse_int_value( line_tok.get_next_token() );
			std::string tile_name = line_tok.get_next_token();
			bool tile_show_in_browser = w_parser::parse_bool_value( line_tok.get_next_token() );

			tile_masters[ tile_id ].id = tile_id;
			tile_masters[ tile_id ].show_in_browser = tile_show_in_browser;
			tile_masters[ tile_id ].img = engine->get_asset<a_image>( tile_name );
		}

		line = tok.get_next_token();
	}

	font = engine->get_asset<a_font>( "medieval_font" );

	for( int x = 0 ; x < ROOM_W * ROOM_H ; ++x )
		test_room.tiles[ x ] = engine->random->geti_range( 0, 255 );
}

void g_custom_game::new_game()
{
	w_game::new_game();

	player = game->spawn_entity<e_player>( w_vec3::zero, 30.f );
}

void g_custom_game::update()
{
	w_game::update();

	for( int x = 0 ; x < ROOM_W * ROOM_H ; ++x )
		test_room.tiles[ x ] = engine->random->geti_range( 0, 255 );
}

w_tile* g_custom_game::get_tile( int id )
{
	assert( id > -1 );
	assert( id < 256 );

	w_tile* tile = &( tile_masters[ id ] );
	assert( tile->img != nullptr );	// there is no tile definition at the requested index - double check "tile_def.txt"
	
	return tile;
}
