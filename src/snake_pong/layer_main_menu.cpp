
#include "app_header.h"
// ----------------------------------------------------------------------------

void layer_main_menu_ui_callback::was_left_clicked( w_imgui_control* control )
{
	if( control->tag == "button_play" )
	{
		engine->layer_mgr->pop();
		engine->layer_mgr->push<layer_game>();
		game->new_game();
	}
	else if( control->tag == "button_quit" )
	{
		engine->is_running = false;
	}
}

// ----------------------------------------------------------------------------

layer_main_menu::layer_main_menu()
{
}

void layer_main_menu::draw_ui()
{
	w_layer::draw_ui();

	RENDER
		->begin()
		->push_scale( { 2.0f, 2.0f } )
		->push_align( align::hcenter )
		->push_rgb( w_color::white )
		->push_glow( 100.0f )
		->draw_string( "Light Snake", w_rect( v_window_hw, 32 ) )
		->end();

	float xpos = 64;
	float ypos = 75;

	IMGUI->init_push_button( "button_play" )
		->set_label( "PLAY" )
		->set_position( { xpos, ypos } )
		->set_size( { v_window_w - xpos * 2, 24.0f } )
		->finalize();

	xpos += 16;
	ypos += 28;

	IMGUI->init_push_button( "button_quit" )
		->set_label( "Quit" )
		->set_position( { xpos, ypos } )
		->set_size( { v_window_w - xpos * 2.0f, 24.0f } )
		->finalize();
}

void layer_main_menu::push()
{
	w_layer::push();

	w_entity* e = nullptr;
	w_entity_component* ec = nullptr;

	// particles

	e = add_entity<w_entity>();
	e->get_tform()->set_position( { v_window_hw, 0.0f } );
	e->add_component<ec_emitter>()
		->init( "menu_fire_up" )
		->tform.set( { 0.0f, v_window_h }, 0.0f, 1.0f );

	// mechanical meshes

	e = add_entity<w_entity>()->set_tag( "wheels" );
	e->get_tform()->set_position( { v_window_hw, v_window_hh } )->set_scale( 0.25f );

	e->add_component<ec_mesh>()
		->init( "mesh_torus_test" )
		->tform.set_scale( 0.5f )->set_angle( 120.0f );

	e->add_component<ec_mesh>()
		->init( "mesh_torus_test" )
		->tform.set_position( { 96.0f, 0.0f } )->set_scale( 0.25f );
}

void layer_main_menu::becoming_top_layer()
{
	w_layer::becoming_top_layer();

	engine->window->set_mouse_mode( mouse_mode::custom );
	engine->ui->mouse_cursor = a_cursor::find( "mouse_cursor_default" );
}

bool layer_main_menu::iir_on_pressed( const w_input_event* evt )
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

bool layer_main_menu::iir_on_held( const w_input_event* evt )
{
	static float angle = 0.0f;

	if( evt->input_id == input_id::gamepad_button_left_shoulder )
	{
		angle -= 5.0f;
		find_entity( "wheels" )->get_tform()->set_angle( angle );
	}

	if( evt->input_id == input_id::gamepad_button_right_shoulder )
	{
		angle += 5.0f;
		find_entity( "wheels" )->get_tform()->set_angle( angle );
	}

	return false;
}

bool layer_main_menu::iir_on_motion( const w_input_event* evt )
{
	static w_vec2 wheel_01_pos = { v_window_hw, v_window_hh };

	if( evt->input_id == input_id::gamepad_left_stick )
	{
		wheel_01_pos += evt->delta;
		find_entity( "wheels" )->get_tform()->set_position( wheel_01_pos );
	}

	return false;
}

w_imgui_callback* layer_main_menu::get_imgui_callback()
{
	return &imgui_callback;
}
