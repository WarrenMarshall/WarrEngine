
#include "app_header.h"

mario_fun_game::mario_fun_game( std::string_view name )
	: w_game( name )
{
}

void mario_fun_game::init()
{
	w_game::init();

	snd_plat_jump = engine->get_asset<a_sound>( "snd_plat_jump" );
	snd_plat_coin = engine->get_asset<a_sound>( "snd_plat_coin" );
	snd_plat_drop_down = engine->get_asset<a_sound>( "snd_plat_drop_down" );

	music_main_menu = engine->get_asset<a_sound>( "music_main_menu" );

	tile_set_subtex =
	{
		engine->get_asset<a_subtexture>( "tile_01" ),
		engine->get_asset<a_subtexture>( "tile_02" ),
		engine->get_asset<a_subtexture>( "tile_03" ),
		engine->get_asset<a_subtexture>( "tile_04" ),
		engine->get_asset<a_subtexture>( "tile_05" ),
		engine->get_asset<a_subtexture>( "tile_06" ),
		engine->get_asset<a_subtexture>( "tile_07" ),
		engine->get_asset<a_subtexture>( "tile_08" ),
		engine->get_asset<a_subtexture>( "tile_09" ),
		engine->get_asset<a_subtexture>( "tile_10" ),
		engine->get_asset<a_subtexture>( "tile_11" ),
		engine->get_asset<a_subtexture>( "tile_12" ),
		engine->get_asset<a_subtexture>( "tile_13" )
	};
}

void mario_fun_game::new_game()
{
	w_game::new_game();
}

void mario_fun_game::reset_layer_stack_to_main_menu()
{
	engine->layer_mgr->clear_stack();
	engine->layer_mgr->push<layer_main_menu>();

	player = nullptr;
	player_camera = nullptr;
}

std::string mario_fun_game::get_game_name()
{
	return fmt::format( "{}", ADD_QUOTES( APP_NAME ) );
}

void mario_fun_game::load_level( std::string_view level_filename )
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
	// player

	assert( player );	// should have been spawned above. did you forget to add a player start?

	// ----------------------------------------------------------------------------
	// camera

	player_camera = LAYER->add_entity<w_camera>();
	player_camera->pos = player->pos;
	player_camera->set_follow_target( player, follow_flags::xy_axis, 0.10f );
	player_camera->set_follow_limits_x( { 160.0f, 288.0f } );
	player_camera->set_follow_limits_y( { 0.0f, 316.0f } );

	// ----------------------------------------------------------------------------
	// some random coins

	//spawn_coins();
}

void mario_fun_game::spawn_coins()
{
	for( int c = 0 ; c < 12 ; ++c )
	{
		auto coin = LAYER->add_entity<e_platformer_coin>();
		coin->set_position_deep( { w_random::getf_range( 16.0f, v_window_w - 32 ), w_random::getf_range( 16.0f, 32.0f ) }, true );
	}
}
