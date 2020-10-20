
#include "app_header.h"

// ----------------------------------------------------------------------------

platformer_layer::platformer_layer()
{
	draws_completely_solid = true;

	music_gameplay = engine->get_asset<a_sound>( "music_gameplay" );
}

void platformer_layer::push()
{
	w_layer::push();

	sky_gradient = engine->get_asset<a_gradient>( "sky_gradient" );

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
