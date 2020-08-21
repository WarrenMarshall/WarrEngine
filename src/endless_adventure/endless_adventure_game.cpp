
#include "app_header.h"

endless_adventure_game::endless_adventure_game()
{
}

void endless_adventure_game::init()
{
	auto file = engine->fs->load_text_file_into_memory( "endless_adventure_data\\tiles\\tile_def.txt" );

	for( const auto& line : *(file.get()->lines.get()) )
	{
		w_tokenizer line_tok( line, ',', "" );

		int tile_master_idx = w_parser::int_from_str( line_tok.get_next_token() );
		std::string_view tile_name = line_tok.get_next_token();

		std::string_view str = line_tok.get_next_token();

		e_area_layer area_layer = area_layer::nobrowse;
		if( str == "geometry" )
			area_layer = area_layer::geometry;
		else if( str == "item" )
			area_layer = area_layer::item;
		else if( str == "enemy" )
			area_layer = area_layer::enemy;

		tile_masters[ tile_master_idx ].id = tile_master_idx;
		tile_masters[ tile_master_idx ].area_layer = area_layer;
		tile_masters[ tile_master_idx ].subtex = engine->get_asset<a_subtexture>( tile_name );
	}

	// populate the rooms with default tile values

	for( auto& iter : areas )
	{
		for( int x = 0 ; x < AREA_SZ * AREA_SZ ; ++x )
		{
			iter.tile_ids[ area_layer::geometry ][ x ] = 4; // tile_grass
			iter.tile_ids[ area_layer::item ][ x ] = empty_tile;
			iter.tile_ids[ area_layer::enemy ][ x ] = empty_tile;
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

void endless_adventure_game::load_area_data()
{
	FILE* file = nullptr;
	fopen_s( &file, "area_data.dat", "rb" );
	if( file == nullptr )
	{
		log_msg( fmt::format( "{} : failed", __FUNCTION__ ) );
	}
	else
	{
		std::fread( GAME->areas, sizeof( w_area ), 10, file );
		std::fclose( file );
	}
}

void endless_adventure_game::save_area_data()
{
	FILE* file = nullptr;
	fopen_s( &file, "area_data.dat", "wb" );
	if( file == nullptr )
	{
		log_msg( fmt::format( "{} : failed", __FUNCTION__ ) );
	}
	else
	{
		std::fwrite( GAME->areas, sizeof( w_area ), 10, file );
		std::fclose( file );
	}
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
