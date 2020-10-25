
#include "app_header.h"

// ----------------------------------------------------------------------------

platformer_layer::platformer_layer()
{
	draws_completely_solid = true;

	music_gameplay = a_sound::find( "music_gameplay" );
}

void platformer_layer::push()
{
	w_layer::push();

	sky_gradient = a_gradient::find( "sky_gradient" );

	engine->window->set_mouse_mode( mouse_mode::hidden );

	plat_physics = std::make_unique<w_platformer_physics>();
	engine->box2d_world->SetContactListener( plat_physics.get() );

	auto e = add_entity<w_entity>();
	e->add_component<ec_emitter>()->init( "background_stars" );
	e->set_position( { v_window_hw, 256.0f } );

	//music_gameplay->play();
}

void platformer_layer::pop()
{
	w_layer::pop();

	//music_gameplay->stop();

	engine->box2d_world->SetContactListener( nullptr );
}

void platformer_layer::update()
{
	plat_physics->handle_user_input();
	plat_physics->update();

	w_layer::update();
}

void platformer_layer::draw()
{
	// draw the background gradient at the camera position
	RENDER->draw( sky_gradient, w_rect( game->player_camera->pos.x - v_window_hw, 0, v_window_w, v_window_h * 2 ) );

	w_layer::draw();
}

void platformer_layer::draw_ui_debug()
{
	RENDER
		->begin()
		->draw_string( engine->pixel_font, fmt::format( "on_ground : {}", plat_physics->player_on_ground ), w_rect( 8, 8 ) )
		->draw_string( engine->pixel_font, fmt::format( "drop_down_blocked : {}", plat_physics->player_drop_down_blocked ), w_rect( 8, 16 ) )
		->draw_string( engine->pixel_font, fmt::format( "vel : {:.1f}, {:.1f}", plat_physics->vel_x, plat_physics->vel_y ), w_rect( 8, 24 ) )
		->end();
}

bool platformer_layer::handle_input_event( const w_input_event* evt )
{
	if( evt->event_id == event_id::input_pressed )
	{
		if( evt->input_id == input_id::key_1 )
		{
			game->player->set_position_deep( engine->input->mouse_vwindow_pos, true );
			return true;
		}

		if( evt->input_id == input_id::key_n )
		{
			game->spawn_coins();
			return true;
		}

		plat_physics->handle_input_event( evt );
	}
	return true;
}

w_camera* platformer_layer::get_camera()
{
	return game->player_camera;
}
