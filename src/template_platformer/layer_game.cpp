
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

	engine->window->set_mouse_mode( mouse_mode::locked );

	auto e = add_entity<w_entity>();
	e->add_component<ec_emitter>()->init( "background_stars" );
	e->it_set_position( { v_window_hw, 256.0f } );

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
	auto player = LAYER->find_entity_from_tag( "player" );

	if( !player )
	{
		return;
	}

	auto phys_responder = (w_platformer_physic_responder*) game->physics_responder.get();
	phys_responder->update();

	w_layer::update();

	auto ec = player->get_component<ec_sprite>( component_type::sprite );
	ec->tex = a_anim_texture::find( "anim_player_idle" );

	auto ec_b2d = player->phys_get_primary_body();
	w_vec2 vel = w_vec2( ec_b2d->body->GetLinearVelocity() ).from_b2d();

	if( !phys_responder->on_ground() )
	{
		ec->tex = a_anim_texture::find( "anim_player_jump" );
	}
	else
	{
		if( !fequals( vel.x, 0.0f ) )
		{
			ec->tex = a_anim_texture::find( "anim_player_run" );
		}
	}
}

void platformer_layer::draw()
{
	// draw the background gradient at the camera position
	RENDER->draw( sky_gradient, w_rect( game->player_camera->pos.x - v_window_hw, 0, v_window_w, v_window_h * 2 ) );

	w_layer::draw();
}

void platformer_layer::draw_ui_debug()
{
	auto phys_responder = (w_platformer_physic_responder*) game->physics_responder.get();

	RENDER
		->begin()
		->draw_string( engine->pixel_font, fmt::format( "on_ground : {}", phys_responder->player_on_ground ), w_rect( 8, 8 ) )
		->draw_string( engine->pixel_font, fmt::format( "drop_down_blocked : {}", phys_responder->player_drop_down_blocked ), w_rect( 8, 16 ) )
		->draw_string( engine->pixel_font, fmt::format( "vel : {:.1f}, {:.1f}", phys_responder->vel_x, phys_responder->vel_y ), w_rect( 8, 24 ) )
		->end();
}

w_camera* platformer_layer::get_camera()
{
	return game->player_camera;
}

bool platformer_layer::iir_on_pressed( const w_input_event* evt )
{
	if( game->player_input_controller->iir_on_pressed( evt ) )
	{
		return true;
	}

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

	return false;
}

bool platformer_layer::iir_on_motion( const w_input_event* evt )
{
	if( game->player_input_controller->iir_on_motion( evt ) )
	{
		return true;
	}

	return false;
}
