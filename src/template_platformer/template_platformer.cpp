
#include "app_header.h"

template_platformer::template_platformer( const char* name )
	: w_game( name )
{
}

void template_platformer::init()
{
	w_game::init();

	snd_jump = a_sound::find( "snd_jump" );
	snd_coin = a_sound::find( "snd_coin" );
	snd_drop_down = a_sound::find( "snd_drop_down" );

	music_main_menu = a_sound::find( "music_main_menu" );

	tile_set_subtex =
	{
		a_subtexture::find( "tile_01" ),
		a_subtexture::find( "tile_02" ),
		a_subtexture::find( "tile_03" ),
		a_subtexture::find( "tile_04" ),
		a_subtexture::find( "tile_05" ),
		a_subtexture::find( "tile_06" ),
		a_subtexture::find( "tile_07" ),
		a_subtexture::find( "tile_08" ),
		a_subtexture::find( "tile_09" ),
		a_subtexture::find( "tile_10" ),
		a_subtexture::find( "tile_11" ),
		a_subtexture::find( "tile_12" ),
		a_subtexture::find( "tile_13" )
	};
}

void template_platformer::reset_layer_stack_to_main_menu()
{
	engine->layer_mgr->clear_stack();
	engine->layer_mgr->push<layer_main_menu>();

	player = nullptr;
	player_camera = nullptr;
}

void template_platformer::load_level( std::string_view level_filename )
{
	ec_b2d_body* ec = nullptr;

	// ----------------------------------------------------------------------------
	// world geometry

	auto world = LAYER->add_entity<e_platformer_level>();
	auto ec_tm = world->get_component<ec_tilemap>( component_type::tilemap );
	ec_tm->load_from_disk( "world", tile_set_subtex, level_filename );

	// replace all tiles that are pickups with actual entities

	for( auto& tm_layer : ec_tm->tile_layers )
	{
		for( auto x = 0 ; x < tm_layer->tiles.size() ; ++x )
		{
			ec_tilemap_tile& tile = tm_layer->tiles[ x ];

			if( tile.tileset_idx == 0 )
			{
				// coin pickup
				auto coin = LAYER->add_entity<e_platformer_coin>();
				coin->set_position_deep( { tile.pos.x + 8.0f, tile.pos.y + 8.0f }, true );

				// remove tile from map
				tm_layer->tiles.erase( tm_layer->tiles.begin() + x );
				x--;
			}
			else if( tile.tileset_idx == 12 )
			{
				// player start
				player = LAYER->add_entity<e_platformer_player>();
				player->set_position_deep( { tile.pos.x + 8.0f, tile.pos.y + 8.0f }, true );
				player->set_position( { v_window_hw, tile.pos.y + 8.0f } );

				// remove tile from map
				tm_layer->tiles.erase( tm_layer->tiles.begin() + x );
				x--;
			}
		}
	}

	// ----------------------------------------------------------------------------
	// player should have been spawned above.
	// did you forget to add a player start to your level?
	assert( player );

	// ----------------------------------------------------------------------------
	// camera

	player_camera = LAYER->add_entity<w_camera>();
	player_camera->pos = player->pos;
	player_camera->set_follow_target( player, follow_flags::xy_axis, 3.0f );
	player_camera->set_follow_limits_x( { 160.0f, 288.0f } );
	player_camera->set_follow_limits_y( { 0.0f, 316.0f } );

	// ----------------------------------------------------------------------------
	// some random coins

	//spawn_coins();
}

void template_platformer::spawn_coins()
{
	for( int c = 0 ; c < 12 ; ++c )
	{
		auto coin = LAYER->add_entity<e_platformer_coin>();
		coin->set_position_deep( { w_random::getf_range( 16.0f, v_window_w - 32 ), w_random::getf_range( 16.0f, 32.0f ) }, true );
	}
}
