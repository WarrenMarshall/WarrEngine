
#include "app_header.h"

mario_fun_app::mario_fun_app( std::string_view name )
	: w_game( name )
{
}

void mario_fun_app::init()
{
	w_game::init();

	snd_plat_jump = engine->get_asset<a_sound>( "snd_plat_jump" );
	snd_plat_coin = engine->get_asset<a_sound>( "snd_plat_coin" );
	snd_plat_drop_down = engine->get_asset<a_sound>( "snd_plat_drop_down" );

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
		engine->get_asset<a_subtexture>( "tile_12" )
	};
}

void mario_fun_app::new_game()
{
	w_game::new_game();

	return_to_main_menu();
}

void mario_fun_app::return_to_main_menu()
{
	engine->layer_mgr->clear_stack();
	engine->layer_mgr->push<layer_main_menu>();
}

std::string mario_fun_app::get_game_name()
{
	return fmt::format( "{}", ADD_QUOTES( APP_NAME ) );
}

void mario_fun_app::load_level( std::string_view level_filename )
{
	w_layer* layer = engine->layer_mgr->get_top();

	ec_b2d_body* ec = nullptr;

	// ----------------------------------------------------------------------------
	// world geometry

	auto world = layer->add_entity<e_platformer_level>();
	auto ec_tm = world->get_component<ec_tilemap>( component_type::tilemap );
	ec_tm->load_from_disk( "world", tile_set_subtex, level_filename );

	// ----------------------------------------------------------------------------
	// player

	player = layer->add_entity<e_platformer_player>();
	player->set_position_deep( { v_window_hw, 16.0f }, true );

	// ----------------------------------------------------------------------------
	// camera

	auto player_camera = layer->add_entity<w_camera>();
	player_camera->pos = player->pos;
	player_camera->set_follow_target( player, follow_flags::y_axis, 0.05f );
	RENDER->current_camera = player_camera;

	// ----------------------------------------------------------------------------
	// some random coins

	//spawn_coins();
}

void mario_fun_app::spawn_coins()
{
	auto layer = engine->layer_mgr->get_top();

	for( int c = 0 ; c < 12 ; ++c )
	{
		auto coin = layer->add_entity<e_platformer_coin>();
		coin->set_position_deep( { w_random::getf_range( 16.0f, v_window_w - 32 ), w_random::getf_range( 16.0f, 32.0f ) }, true );
	}
}
