
#include "master_pch.h"
#include "master_header.h"

// ------------------------------------------------------------------------------

void layer_editor::push()
{
	engine->input_mgr->add_listener( this );
	//tween_pingpong = std::make_unique<w_tween>( e_tween_type::pingpong, -150.0f, 150.0f, 15.0f );
	//tween_rotate = std::make_unique<w_tween>( e_tween_type::loop, 0.0f, 360.0f, 50.0f );

	//pingpong_xform.set_transform( w_vec3( 0, -72, 0 ), 0, 1.0f );
	//rotate_xform.set_transform( w_vec3( 0, -96, 0 ), 0, 1.0f );
	selector_bracket = std::make_unique<a_image>( "selector_bracket" );
	mouse_cursor = engine->get_asset<a_cursor>( "ui_cursor" );

	engine->input_mgr->set_mouse_mode( e_mouse_mode::hidden );
}

void layer_editor::pop()
{
	engine->input_mgr->remove_listener( this );

	engine->input_mgr->set_mouse_mode( e_mouse_mode::normal );
}

void layer_editor::update()
{
	w_layer::update();

	//tween_pingpong->update();
	//pingpong_xform.pos.x = tween_pingpong->get_fval();
	//
	//tween_rotate->update();
	//rotate_xform.angle = tween_rotate->get_fval();
}

void layer_editor::draw()
{
	w_layer::draw();

	game->draw_entities();

	if( draw_selector_bracket )
	{
		MATRIX
			->push_identity()
			->translate( w_vec3( -v_window_hw, v_window_hh - ( TILE_SZ * 3 ), 0 ) )
			->translate( w_vec3( ( hover_tile.x * TILE_SZ ), -( hover_tile.y * TILE_SZ ), 0 ) );

		RENDER->draw( selector_bracket.get() );

		MATRIX->pop();
	}

	// mouse cursor test

	MATRIX
		->push_identity()
		->translate( w_vec3(
			-v_window_hw + engine->input_mgr->mouse_vwindow_pos.x - mouse_cursor->hotspot_offset.x,
			v_window_hh - engine->input_mgr->mouse_vwindow_pos.y - mouse_cursor->hotspot_offset.y,
			1000 ) );

	RENDER->draw( mouse_cursor->img );

	MATRIX->pop();

	// ----------------------------------------------------------------------------

	/*
	float step_per_fts = tween_pingpong->step_per_sec / w_time::FTS_desired_frames_per_second;
	step_per_fts *= tween_pingpong->_dir;

	OPENGL
		->push( true )
		->add_transform( pingpong_xform )
		->translate( w_vec3( step_per_fts * RENDER->frame_interpolate_pct, 0, 0 ) );
	RENDER
		->begin()
		->push_color( W_COLOR_TEAL )
		->draw_sprite( engine->white_solid )
		->end();
	OPENGL
		->pop();

	// ----------------------------------------------------------------------------

	step_per_fts = tween_rotate->step_per_sec / w_time::FTS_desired_frames_per_second;

	OPENGL
		->push( true )
		->add_transform( rotate_xform )
		->rotate( step_per_fts * RENDER->frame_interpolate_pct );
	RENDER
		->begin()
		->push_color( W_COLOR_ORANGE )
		->draw_sprite( engine->white_solid )
		->end();
	OPENGL
		->pop();
	*/
}

void layer_editor::on_listener_event_received( e_event_id event, void* object )
{
	const w_input_event* evt = static_cast<w_input_event*>( object );

	switch( event )
	{
		case e_event_id::input_motion:
		{
			tile_from_screen_pos( evt->data.xpos, evt->data.ypos );
			//log_msg( "%1.2f, %1.2f", evt->data.xpos, evt->data.ypos );
		}
		break;

		case e_event_id::input_pressed:
		{
			switch( evt->data.input_id )
			{
				case e_input_id::mouse_button_right:
				{
					w_fibonacci fib;
					std::string seq = "";

					for( int x = 0 ; x < 50 ; ++x )
					{
						seq += s_format( "%d", fib.step() );
					}
					log_msg( "%s", seq.c_str() );
				}
				break;
			}
		}
		break;
	}
}

// takes a position within the game viewport and converts it into
// a tile index within the current room

int layer_editor::tile_from_screen_pos( float xpos, float ypos )
{
	//w_world_room* room = &game->test_room;

	float tiles_x = 0;
	float tiles_y = TILE_SZ * 2;

	int tile_x = static_cast<int>( ( xpos - tiles_x ) / (float)TILE_SZ );
	int tile_y = static_cast<int>( ( ypos - tiles_y ) / (float)TILE_SZ );

	//log_msg( "%d,%d", tile_x, tile_y );

	hover_tile = w_vec2( static_cast<float>( tile_x ), static_cast<float>( tile_y ) );
	draw_selector_bracket = ( tile_x >= 0 && tile_x < 19 && tile_y >= 0 && tile_y < 9 );

	return 0;
}
