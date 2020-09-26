
#include "app_header.h"

// ----------------------------------------------------------------------------

layer_platformer::layer_platformer()
{
	draws_completely_solid = true;
}

void layer_platformer::push()
{
	w_layer::push();

	plat_physics = std::make_unique<w_platformer_physics>();
	engine->box2d_world->SetContactListener( plat_physics.get() );

	ec_b2d_body* ec = nullptr;

	// world geometry

	world_geo = add_entity<w_entity>();
	world_geo->collision_layer = clayer_world;
	world_geo->collides_with = clayer_player;
	world_geo->draw_debug_info = true;
	ec = world_geo->add_component<ec_b2d_static>();
	{
		// bounding box for world

		ec->add_fixture_line_loop(
			sensor_id::world,
			w_vec2::zero,
			{
				{ 4.0f, 4.0f },
				{ v_window_w - 8.0f, 4.0f },
				{ v_window_w - 8.0f, v_window_h - 8.0f },
				{ 4.0f, v_window_h - 8.0f }
			}
		);

		// rando lines running vertically

		for( int y = 32 ; y < v_window_h ; y += 30 )
		{
			float xpos = w_random::getf_range( 0, v_window_hw );
			auto ypos = (float) y;
			float w = w_random::getf_range( 64, v_window_hw );
			ec->add_fixture_line( sensor_id::world, w_vec2::zero, { xpos, ypos }, { xpos + w, ypos } );
		}

		for( int x = 0 ; x < 10 ; ++x )
		{
			float xpos = w_random::getf_range( 0.0f, v_window_w );
			float ypos = w_random::getf_range( 64.0f, v_window_h );

			if( w_random::geti_range( 0, 4 ) == 2 )
			{
				float sz = w_random::getf_range( 4, 16 );
				ec->add_fixture_circle( sensor_id::world, { xpos, ypos }, sz );
			}
			else
			{
				float sz = w_random::getf_range( 4, 32 );
				float sz2 = w_random::getf_range( 4, 32 );
				ec->add_fixture_box( sensor_id::world, w_rect( xpos, ypos, sz, sz2 ) );
			}
		}
	}

	// ----------------------------------------------------------------------------

	player = add_entity<e_platformer_player>();
}

void layer_platformer::pop()
{
	w_layer::pop();

	engine->box2d_world->SetContactListener( nullptr );
}

void layer_platformer::update()
{
	w_layer::update();
	plat_physics->move_player( player );
}

void layer_platformer::draw()
{
	w_layer::draw();

	RENDER
		->begin()
		->push_rgb( w_color::teal );

	RENDER->draw_string( engine->pixel_font, fmt::format( "on ground   : {}", !plat_physics->in_air() ), w_rect( 16, 16 ) );
	RENDER->draw_string( engine->pixel_font, fmt::format( "drop blocked: {}", !plat_physics->can_drop_down() ), w_rect( 16, 24 ) );

	RENDER->end();
}

bool layer_platformer::handle_input_event( const w_input_event* evt )
{
	if( evt->event_id == event_id::input_pressed )
	{
		if( evt->input_id == input_id::controller_button_a )
		{
			plat_physics->jump_player( player );
		}

		if( evt->input_id == input_id::key_1 )
		{
			player->teleport( engine->input->mouse_vwindow_pos, true );
		}
	}
	return true;
}
