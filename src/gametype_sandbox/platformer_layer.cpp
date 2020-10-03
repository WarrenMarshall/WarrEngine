
#include "app_header.h"

// ----------------------------------------------------------------------------

platformer_layer::platformer_layer()
{
	draws_completely_solid = true;
}

void platformer_layer::push()
{
	w_layer::push();

	test_tween = tweeny::from( 0.0f ).to( 1.0f ).during( 2000 ).via( tweeny::easing::linear );

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
			"world",
			w_vec2::zero,
			{
				{ 4.0f, 4.0f },
				{ v_window_w - 8.0f, 4.0f },
				{ v_window_w - 8.0f, v_window_h - 8.0f },
				{ 4.0f, v_window_h - 8.0f }
			}
		);

#if 1
		// rando platform lines running vertically

		for( int y = 0 ; y < v_window_h ; y += 40 )
		{
			float xpos = 8.0f;
			auto ypos = (float) y;

			float w = w_random::getf_range( 32, 120 );
			ec->add_fixture_line( "world", w_vec2::zero, { xpos, ypos }, { xpos + w, ypos } );

			xpos = v_window_w - 8.0f;
			w = w_random::getf_range( 32, 120 );
			ec->add_fixture_line( "world", w_vec2::zero, { xpos - w, ypos }, { xpos, ypos } );
		}
#endif
	}

	// ----------------------------------------------------------------------------

	player = add_entity<e_platformer_player>();
	player->set_position_deep( { v_window_hw, 16.0f }, true );

	// ----------------------------------------------------------------------------

	spawn_coins();

	// ----------------------------------------------------------------------------

	tween_mover = tweeny::from(-1.0f ).to( 1.0f ).during( 1000 );

	mover = add_entity<e_platformer_mover>();
	mover->set_position_deep( { v_window_hw, v_window_hh }, true );
}

void platformer_layer::pop()
{
	w_layer::pop();

	engine->box2d_world->SetContactListener( nullptr );
}

void platformer_layer::update()
{
	test_tween.step( (int) w_time::FTS_step_value_ms );

	plat_physics->handle_user_input( player );
	plat_physics->update();

	tween_mover.step( (int) w_time::FTS_step_value_ms );

	auto ekb = mover->get_component<ec_b2d_kinematic>( component_type::b2d_body );
	ekb->body->SetLinearVelocity( w_vec2( 0.0f, tween_mover.peek() * 1.5f ) );

	w_layer::update();
}

void platformer_layer::draw()
{
	w_layer::draw();

	auto ec = player->get_component<ec_b2d_body>( component_type::b2d_dynamic | component_type::b2d_kinematic );

	RENDER
		->begin()
		->push_rgba( w_color::teal, 0.5f );

	RENDER->draw_string( engine->pixel_font, fmt::format( "'1' - set_position_forced player", !plat_physics->in_air() ), w_rect( 16, 16 ) );
	RENDER->draw_string( engine->pixel_font, fmt::format( "'N' - drop more coins", !plat_physics->can_drop_down() ), w_rect( 16, 24 ) );
	RENDER->draw_string( engine->pixel_font, fmt::format( "Tween : {:.2f}", test_tween.peek() ), w_rect( 16, 32 ) );

	RENDER->end();
}

bool platformer_layer::handle_input_event( const w_input_event* evt )
{
	if( evt->event_id == event_id::input_pressed )
	{
		//if( evt->input_id == input_id::controller_button_a )
		//{
		//	plat_physics->jump_player( player );
		//}

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

