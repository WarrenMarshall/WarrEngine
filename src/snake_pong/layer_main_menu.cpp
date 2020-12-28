
#include "app_header.h"

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
		->push_emissive( 100.0f )
		->draw_string( "Light Snake", w_rect( v_window_hw, 32 ) )
		->end();

	float xpos = 64;
	float ypos = 75;

	if( IMGUI->init_push_button()
		->set_label( "PLAY" )
		->set_slice_def( a_9slice_def::find( "sd_push_button" ) )
		->set_rect( { xpos, ypos, v_window_w - xpos * 2, 24 } )
		->finalize()
		->was_left_clicked() )
	{
		engine->layer_mgr->pop();
		engine->layer_mgr->push<layer_game>();
		game->new_game();
	}

	xpos += 16;
	ypos += 28;

	if( IMGUI->init_push_button()
		->set_label( "Quit" )
		->set_slice_def( a_9slice_def::find( "sd_push_button" ) )
		->set_rect( { xpos, ypos, v_window_w - xpos * 2, 24 } )
		->finalize()
		->was_left_clicked() )
	{
		engine->is_running = false;
	}
}

void layer_main_menu::push()
{
	w_layer::push();

	w_entity* e;

	// particles

	e = add_entity<w_entity>();
	e->it_set_position( { v_window_hw, 0.0f } );
	e->add_component<ec_emitter>()->init( "menu_fire_up" )
		->it_set( { 0.0f, v_window_h }, 0.0f, 1.0f );

	//e = add_entity<w_entity>();
	//e->it_set_position( { v_window_hw, v_window_hh } );
	//e->add_component<ec_mesh>()->init( "mesh_torus_test" )
	//	->it_set_scale( 0.5f );
	//e->add_component<ec_mesh>()->init( "mesh_torus_test" )
	//	->it_set_scale( 0.25f );
}

void layer_main_menu::becoming_top_layer()
{
	w_layer::becoming_top_layer();

	engine->window->set_mouse_mode( mouse_mode::custom );
	engine->ui->mouse_cursor = a_cursor::find( "mouse_cursor_default" );
}

void layer_main_menu::new_game()
{
	w_layer::new_game();

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
