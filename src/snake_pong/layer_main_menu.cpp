
#include "app_header.h"

// ----------------------------------------------------------------------------

void layer_main_menu_ui_callback::on_left_clicked( const w_imgui_control& control, const w_imgui_result& result )
{
	switch( control.tag )
	{
		case H( "button_play" ):
		{
			engine->layer_mgr->pop();
			engine->layer_mgr->push<layer_game>();
			game->new_game();
		}
		break;

		case H( "button_quit" ):
		{
			engine->is_running = false;
		}
		break;
	}
}

// ----------------------------------------------------------------------------

layer_main_menu::layer_main_menu()
{
}

void layer_main_menu::draw_ui()
{
	w_layer::draw_ui();

	w_render_state_opt rso;
	rso.scale = { 2.0f, 4.0f };
	rso.align = align::hcenter;
	rso.color = w_color::white;
	rso.glow = 0.25f;

	RENDER
		->begin()
		->push_render_state( rso )
		->draw_string( "Pong Paddle Thing", { v_window_hw + *xlogo_tween, 32.0f + *logo_tween } )
		->end();

	float xpos = 64;
	float ypos = 100;

	IMGUI->do_push_button( H( "button_play" ) )
		->set_text( "PLAY" )
		->set_position( { xpos, ypos } )
		->set_size( { v_window_w - xpos * 2.0f, w_sz::def } )
		->finalize();

	xpos += 16;
	ypos += 28;

	IMGUI->do_push_button( H( "button_quit" ) )
		->set_text( "Quit" )
		->set_position( { xpos, ypos } )
		->set_size( { v_window_w - xpos * 2.0f, w_sz::def } )
		->finalize();
}

void layer_main_menu::push()
{
	w_layer::push();

	w_entity* e = nullptr;

	// mechanical meshes

	e = add_entity<w_entity>();
	{
		e->tag = H( "wheels" );
		e->get_tform()->set_position( { v_window_w, v_window_h } )->set_scale( 0.75 );

		w_render_state_opt rso;
		rso.snap_to_pixel = false;
		rso.color = w_color::black;

		rso.color->a = 0.15f;
		e->add_component<ec_mesh>()
			->init( "mesh_torus_test" )
			->set_tag( H( "right_wheel" ) )
			->set_render_state( rso );

		rso.color->a = 0.15f;
		e->add_component<ec_mesh>()
			->init( "mesh_torus_test" )
			->set_tag( H( "left_wheel" ) )
			->set_render_state( rso )
			->tform.set_position( { -v_window_w - 64, 0.0f } );
	}

	// particles

	e = add_entity<w_entity>();
	{
		e->get_tform()->set_position( { v_window_hw, 0.0f } );
		e->add_component<ec_emitter>()
			->init( "menu_fire_up" )
			->tform.set( { 0.0f, v_window_h }, 0.0f, 1.0f );
		e->add_component<ec_emitter>()->init( "emitter_game_stars" )
			->tform.set( { 0.0f, v_window_hw }, 0.0f, 1.0f );
		e->add_component<ec_emitter>()->init( "emitter_game_stars" )
			->tform.set( { 0.0f, v_window_hw }, 0.0f, 1.0f );
	}

	gear_tween = w_tween( 0.0f, 360.0f, 7500, tween_type::pingpong, tween_via::bounce );
	logo_tween = w_tween( 0.0f, 16.0f, 250, tween_type::pingpong, tween_via::sinusoidal_in );
	xlogo_tween = w_tween( -32.0f, 32.0f, 750, tween_type::pingpong, tween_via::sinusoidal );

	//a_sound::find( "main_menu_music" )->play();
}

void layer_main_menu::pop()
{
	w_layer::pop();
	//a_sound::find( "main_menu_music" )->stop();
}

void layer_main_menu::update()
{
	w_layer::update();

	find_entity( H( "wheels" ) )->get_component( H( "left_wheel" ) )->tform.set_angle( -*gear_tween );
	find_entity( H( "wheels" ) )->get_component( H( "right_wheel" ) )->tform.set_angle( *gear_tween );
}

void layer_main_menu::becoming_top_layer()
{
	w_layer::becoming_top_layer();

	engine->window->set_mouse_mode( mouse_mode::custom );
	engine->ui->mouse_cursor = a_cursor::find( "mouse_cursor_default" );
}

bool layer_main_menu::on_input_pressed( const w_input_event* evt )
{
	if( evt->input_id == input_id::key_space )
	{
		engine->layer_mgr->pop();
		engine->layer_mgr->push<layer_game>();
		game->new_game();

		return true;
	}

	return false;
}

w_imgui_callback* layer_main_menu::get_imgui_callback()
{
	return &imgui_callback;
}
