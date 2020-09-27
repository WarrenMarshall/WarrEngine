
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

	plat_physics = std::make_unique<w_platformer_physics>();
	engine->box2d_world->SetContactListener( plat_physics.get() );

	ec_b2d_body* ec = nullptr;

	// world geometry

	auto ent = add_entity<w_entity>();
	ent->collision_layer = clayer_world;
	ent->collides_with = clayer_player | clayer_coin;
	ent->draw_debug_info = true;
	ec = ent->add_component<ec_b2d_static>();
	{
		// bounding box for world

		ec->add_fixture_line_loop(
			contact_id::world,
			w_vec2::zero,
			{
				{ 4.0f, 4.0f },
				{ v_window_w - 8.0f, 4.0f },
				{ v_window_w - 8.0f, v_window_h - 8.0f },
				{ 4.0f, v_window_h - 8.0f }
			}
		);
	}

	ent = add_entity<w_entity>();
	ent->collision_layer = clayer_world;
	ent->collides_with = clayer_player | clayer_coin;
	ent->draw_debug_info = true;
	ec = ent->add_component<ec_b2d_static>();
	{
		// rando platform lines running vertically

		for( int y = 32 ; y < v_window_h ; y += 30 )
		{
			float xpos = w_random::getf_range( 0, v_window_hw );
			auto ypos = (float) y;
			float w = w_random::getf_range( 64, v_window_hw );
			ec->add_fixture_line( contact_id::world, w_vec2::zero, { xpos, ypos }, { xpos + w, ypos } );
		}

#if 0
		for( int x = 0 ; x < 10 ; ++x )
		{
			float xpos = w_random::getf_range( 0.0f, v_window_w );
			float ypos = w_random::getf_range( 64.0f, v_window_h );

			if( w_random::geti_range( 0, 4 ) == 2 )
			{
				float sz = w_random::getf_range( 4, 16 );
				ec->add_fixture_circle( contact_id::world, { xpos, ypos }, sz );
			}
			else
			{
				float sz = w_random::getf_range( 4, 32 );
				float sz2 = w_random::getf_range( 4, 32 );
				ec->add_fixture_box( contact_id::world, w_rect( xpos, ypos, sz, sz2 ) );
			}
		}
#endif
	}

	// ----------------------------------------------------------------------------

	player = add_entity<e_platformer_player>();
	player->set_position( { 32.0f, 0.0f } );

	// ----------------------------------------------------------------------------

	//spawn_coins();
}

void platformer_layer::pop()
{
	w_layer::pop();

	engine->box2d_world->SetContactListener( nullptr );
}

void platformer_layer::update()
{
	w_layer::update();

	plat_physics->update();
	plat_physics->move_player( player );
}

void platformer_layer::draw()
{
	w_layer::draw();

	RENDER
		->begin()
		->push_rgba( w_color::teal, 0.5f );

	RENDER->draw_string( engine->pixel_font, fmt::format( "'1' - teleport player", !plat_physics->in_air() ), w_rect( 16, 16 ) );
	RENDER->draw_string( engine->pixel_font, fmt::format( "'N' - drop more coins", !plat_physics->can_drop_down() ), w_rect( 16, 24 ) );

	RENDER->end();
}

bool platformer_layer::handle_input_event( const w_input_event* evt )
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

		if( evt->input_id == input_id::key_n )
		{
			spawn_coins();
		}
	}
	return true;
}

void platformer_layer::spawn_coins()
{
	for( int c = 0 ; c < 8 ; ++c )
	{
		auto coin = add_entity<e_platformer_coin>();
		coin->set_position( { w_random::getf_range( 16.0f, v_window_w - 32 ), w_random::getf_range( 16.0f, v_window_h - 32 ) } );
	}
}

