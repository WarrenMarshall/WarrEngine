
#include "app_header.h"

endless_adventure_game::endless_adventure_game()
{
}

void endless_adventure_game::init()
{
	auto file = engine->fs->load_text_file_into_memory( "endless_adventure\\tile_def.txt" );

	for( const auto& line : *(file.get()->lines.get()) )
	{
		w_tokenizer line_tok( line, ',', "" );

		int tile_master_idx = w_parser::int_from_str( line_tok.get_next_token() );
		std::string_view tile_name = line_tok.get_next_token();

		std::string_view str = line_tok.get_next_token();

		e_room_layer room_layer = room_layer::nobrowse;
		if( str == "geometry" )
			room_layer = room_layer::geometry;
		else if( str == "item" )
			room_layer = room_layer::item;
		else if( str == "enemy" )
			room_layer = room_layer::enemy;

		tile_masters[ tile_master_idx ].id = tile_master_idx;
		tile_masters[ tile_master_idx ].room_layer = room_layer;
		tile_masters[ tile_master_idx ].subtex = engine->get_asset<a_subtexture>( tile_name );
	}

	// populate the rooms with default tile values

	for( auto& iter : rooms )
	{
		for( int x = 0 ; x < ROOM_SZ * ROOM_SZ ; ++x )
		{
			iter.tile_ids[ room_layer::geometry ][ x ] = 4; // tile_grass
			iter.tile_ids[ room_layer::item ][ x ] = 71; // tile_blank_03
			iter.tile_ids[ room_layer::enemy ][ x ] = 71; // tile_blank_03
		}
	}

	// initial layer set up

	engine->layer_mgr->push( std::make_unique<layer_background>() );
	engine->layer_mgr->push( std::make_unique<layer_worldviewport>() );
	engine->layer_mgr->push( std::make_unique<layer_editor>() );
}

void endless_adventure_game::new_game()
{
	w_game::new_game();
}

void endless_adventure_game::update()
{
	w_game::update();
}

w_tile* endless_adventure_game::get_tile( int id )
{
	assert( id > -1 );
	assert( id < 256 );

	w_tile* tile = &( tile_masters[ id ] );
	assert( tile->subtex != nullptr );	// there is no tile definition at the requested index - double check "tile_def.txt"
	
	return tile;
}

void endless_adventure_game::draw_viewport_caption( const std::string_view caption, float ypos)
{
	RENDER
		->begin()
		->push_depth_nudge( 20 )
		->push_rgb( w_color::orange )
		->push_align( align::centered )
		->draw_string( UI->theme->large_font, caption, w_rect( v_window_hw, ypos ) )
		->end();
}
