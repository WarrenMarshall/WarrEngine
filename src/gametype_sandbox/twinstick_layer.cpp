
#include "app_header.h"

// ----------------------------------------------------------------------------

constexpr float player_move_force_s = 15.5f;
constexpr float player_base_radius = 8.0f;
constexpr float player_move_force_max = 0.75f;

// ----------------------------------------------------------------------------

twinstick_layer::twinstick_layer()
{
	draws_completely_solid = true;
	music = a_sound::find( "twinstick_music_track" );
}

void twinstick_layer::push()
{
	w_layer::push();

	//music->play();

	engine->window->set_mouse_mode( mouse_mode::locked );

	twinstick_physics = std::make_unique<w_twinstick_physics_responder>();
	engine->box2d_world->SetContactListener( twinstick_physics.get() );

	engine->box2d_world->SetGravity( { 0, 0 } );
	ec_b2d_body* ec = nullptr;

	// world geo

	auto world_geo = add_entity<w_entity>();
	world_geo->collision_layer = clayer_world;
	world_geo->collides_with = clayer_player;

	ec = world_geo->add_component<ec_b2d_static>();
	{
		// bounding box for world

		ec->add_fixture_line_loop(
			"world",
			w_vec2::zero,
			{
				{ -v_window_w, -v_window_h },
				{ v_window_w, -v_window_h },
				{ v_window_w, v_window_h },
				{ -v_window_w, v_window_h }
			}
		);

#if 1
		//  random shapes

		for( int x = 0 ; x < 20 ; ++x )
		{
			float xpos = w_random::getf_range( -v_window_w, v_window_w );
			float ypos = w_random::getf_range( -v_window_h, v_window_h );

			if( w_random::geti_range( 0, 3 ) == 1 )
			{
				float sz = w_random::getf_range( 4, 16 );
				ec->add_fixture_circle( "world", { xpos, ypos }, sz );
			}
			else
			{
				float sz = w_random::getf_range( 4, 32 );
				float sz2 = w_random::getf_range( 4, 32 );
				ec->add_fixture_box( "world", w_rect( xpos, ypos, sz, sz2 ) );
			}
		}
#endif
	}

	// player
	{
		player = add_entity<e_twinstick_player>();
		player->set_position_deep( { 0, 0 }, true );
	}

	// camera

	player_camera = add_entity<w_camera>();
	player_camera->set_follow_target( player, follow_flags::xy_axis, 1.0f );
}

void twinstick_layer::pop()
{
	//music->stop();

	engine->box2d_world->SetContactListener( nullptr );

	w_layer::pop();
}

void twinstick_layer::update()
{
	w_layer::update();

	trace_hit_location = w_vec2::zero;

	/*
	auto player = LAYER->find_entity_from_tag( "player" );
	if( player )
	{
		w_raycast_closest hit;
		if( w_physics_query::trace_closest( player->pos, w_vec2::dir_from_angle( player->angle ), 500, clayer_world, &hit ) )
		{
			trace_hit_location = hit.result.pos;
		}
	}
	*/

	//twinstick_physics->handle_user_input( player );
	//twinstick_physics->update();
}

void twinstick_layer::draw()
{
	w_layer::draw();

	float ypos = 0.0f;
	RENDER
		->begin()
		->push_rgba( w_color::teal, 0.5f )
		->draw_string( engine->pixel_font, "TwinStick Shooter!", w_rect( 12, ypos += 12 ) );

	if( trace_hit_location != w_vec2::zero )
	{
		RENDER
			->push_rgb( w_color::red )
			->push_alpha( 0.15f )
			->draw_line( player->pos, trace_hit_location );
	}

	RENDER
		->end();
}

w_camera* twinstick_layer::get_camera()
{
	return player_camera;
}

bool twinstick_layer::event_input_motion( const w_input_event* evt )
{
	if( evt->input_id == input_id::controller_left_stick )
	{
		auto ec = player->get_component<ec_b2d_body>( component_type::b2d_dynamic | component_type::b2d_kinematic );
		b2Vec2 current = ec->body->GetLinearVelocity();
		current.x += ( player_move_force_s * evt->delta.x ) * w_time::FTS_step_value_s;
		current.y += ( player_move_force_s * evt->delta.y ) * w_time::FTS_step_value_s;
		w_vec2 desired = {
			std::clamp( current.x, -player_move_force_max, player_move_force_max ),
			std::clamp( current.y, -player_move_force_max, player_move_force_max )
		};

		ec->body->SetLinearVelocity( { desired.x, desired.y } );
	}

	if( evt->input_id == input_id::controller_right_stick )
	{
		float angle = w_vec2::angle_from_dir( w_vec2::normalize( evt->delta ) );
		player->set_angle_deep( angle );
	}

	return true;
}

bool twinstick_layer::event_input_pressed( const w_input_event* evt )
{
	return true;
}

bool twinstick_layer::event_input_held( const w_input_event* evt )
{
	if( evt->input_id == input_id::controller_left_stick )
	{

	}

	return true;
}

bool twinstick_layer::event_input_released( const w_input_event* evt )
{
	return true;
}
