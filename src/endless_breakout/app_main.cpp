
#include "app_header.h"

endless_breakout_game::endless_breakout_game()
{
}

void endless_breakout_game::init()
{
	sfx_paddle_impact = engine->get_asset<a_sound>( "paddle_impact" );
	sfx_ball_impact = engine->get_asset<a_sound>( "ball_impact" );
	sfx_deathzone_impact = engine->get_asset<a_sound>( "deathzone_impact" );
	sfx_wall_impact = engine->get_asset<a_sound>( "wall_impact" );
	sfx_brick_impact = engine->get_asset<a_sound>( "brick_impact" );

	brick_textures.push_back( engine->get_asset<a_subtexture>( "sub_brick_blue" ) );
	brick_textures.push_back( engine->get_asset<a_subtexture>( "sub_brick_green" ) );
	brick_textures.push_back( engine->get_asset<a_subtexture>( "sub_brick_grey" ) );
	brick_textures.push_back( engine->get_asset<a_subtexture>( "sub_brick_purple" ) );
	brick_textures.push_back( engine->get_asset<a_subtexture>( "sub_brick_red" ) );
	brick_textures.push_back( engine->get_asset<a_subtexture>( "sub_brick_yellow" ) );
}

void endless_breakout_game::new_game()
{
	w_game::new_game();

	engine->layer_mgr->layer_stack.clear();

	engine->layer_mgr->push( std::make_unique<layer_background>() );
	engine->layer_mgr->push( std::make_unique<layer_gameplay>() );
}
