
#include "app_header.h"

// ----------------------------------------------------------------------------

platformer_layer::platformer_layer()
{
	draws_completely_solid = true;
}

void platformer_layer::push()
{
	w_layer::push();

	engine->window->set_mouse_mode( mouse_mode::normal );
	//mover_controller = std::make_unique<w_mover_controller>( -1.0f, 1.0f, 1500.0f );

	plat_physics = std::make_unique<w_platformer_physics>();
	engine->box2d_world->SetContactListener( plat_physics.get() );

	ec_b2d_body* ec = nullptr;

	// ----------------------------------------------------------------------------
	// world geometry

	auto world = add_entity<e_platformer_level>();
	auto ec_tm = world->get_component<ec_tilemap>( component_type::tilemap );

	std::vector<a_subtexture*> tile_set_subtex =
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

	ec_tm->load_from_disk( "world", tile_set_subtex, "data/mario_fun/levels/level_0.tmx" );

	// ----------------------------------------------------------------------------
	// player

	player = add_entity<e_platformer_player>();
	player->set_position_deep( { v_window_hw, 16.0f }, true );

	// ----------------------------------------------------------------------------
	// some random coins

	//spawn_coins();

	// ----------------------------------------------------------------------------
	// moving platform

	//mover = add_entity<e_platformer_mover>();
	//mover->set_position_deep( { v_window_hw, v_window_hh }, true );
}

void platformer_layer::pop()
{
	w_layer::pop();

	engine->box2d_world->SetContactListener( nullptr );
}

void platformer_layer::update()
{
	plat_physics->handle_user_input( player );
	plat_physics->update();

	//mover_controller->update();

	//auto ekb = mover->get_component<ec_b2d_kinematic>( component_type::b2d_body );
	//float speed = 100.0f * mover_controller->tween->current_val;
	//ekb->body->SetLinearVelocity( w_vec2( 0.0f, speed * (w_time::FTS_step_value_s / 2.0f) ) );

	w_layer::update();
}

void platformer_layer::draw()
{
	w_layer::draw();

	auto ec = player->get_component<ec_b2d_body>( component_type::b2d_dynamic | component_type::b2d_kinematic );

#if 0
	RENDER
		->begin()
		->push_rgba( w_color::teal, 0.5f );

	RENDER->draw_string( engine->pixel_font, fmt::format( "'1' - set_position_forced player", !plat_physics->in_air() ), w_rect( 16, 16 ) );
	RENDER->draw_string( engine->pixel_font, fmt::format( "'N' - drop more coins", !plat_physics->can_drop_down() ), w_rect( 16, 24 ) );

	RENDER->end();
#endif
}

bool platformer_layer::handle_input_event( const w_input_event* evt )
{
	if( evt->event_id == event_id::input_pressed )
	{
		if( evt->input_id == input_id::key_1 )
		{
			player->set_position_deep( engine->input->mouse_vwindow_pos, true );
		}

		if( evt->input_id == input_id::key_n )
		{
			spawn_coins();
		}
	}
	return true;
}

void platformer_layer::spawn_coins()
{
	for( int c = 0 ; c < 12 ; ++c )
	{
		auto coin = add_entity<e_platformer_coin>();
		coin->set_position_deep( { w_random::getf_range( 16.0f, v_window_w - 32 ), w_random::getf_range( 16.0f, v_window_h - 32 ) }, true );
	}
}

