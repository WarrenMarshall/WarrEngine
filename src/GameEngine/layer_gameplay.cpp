
#include "master_pch.h"
#include "master_header.h"

// ------------------------------------------------------------------------------

void layer_gameplay::push()
{
	engine->input_mgr->add_listener( this );
	tween_pingpong = std::make_unique<w_tween>( e_tween_type::pingpong, -150.0f, 150.0f, 15.0f );
	tween_rotate = std::make_unique<w_tween>( e_tween_type::loop, 0.0f, 360.0f, 50.0f );

	pingpong_xform.set_transform( w_vec3( 0, -72, 0 ), 0, 1.0f );
	rotate_xform.set_transform( w_vec3( 0, -96, 0 ), 0, 1.0f );
}

void layer_gameplay::pop()
{
	engine->input_mgr->remove_listener( this );
}

void layer_gameplay::update()
{
	w_layer::update();

	tween_pingpong->update();
	pingpong_xform.pos.x = tween_pingpong->get_fval();
	
	tween_rotate->update();
	rotate_xform.angle = tween_rotate->get_fval();
}

void layer_gameplay::draw()
{
	w_layer::draw();

	game->draw_entities();

	// ----------------------------------------------------------------------------

	float step_per_fts = tween_pingpong->step_per_sec / w_time::FTS_desired_frames_per_second;
	step_per_fts *= tween_pingpong->_dir;

	OPENGL
		->push( true )
		->add_transform( pingpong_xform )
		->translate( w_vec3( step_per_fts * RENDER->frame_interpolate_pct, 0, 0 ) );
	RENDER
		->begin()
		->rs_color( W_COLOR_TEAL )
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
		->rs_color( W_COLOR_ORANGE )
		->draw_sprite( engine->white_solid )
		->end();
	OPENGL
		->pop();
}

void layer_gameplay::on_listener_event_received( e_event_id event, void* object )
{
	w_input_event* evt = static_cast<w_input_event*>( object );

	if( event == e_event_id::input_pressed )
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
}
