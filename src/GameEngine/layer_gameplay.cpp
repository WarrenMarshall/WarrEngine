
#include "master_pch.h"
#include "master_header.h"

// ------------------------------------------------------------------------------

void layer_gameplay::push()
{
	engine->input_mgr->add_listener( this );
}

void layer_gameplay::pop()
{
	engine->input_mgr->remove_listener( this );
}

void layer_gameplay::draw()
{
	w_layer::draw();

	game->draw_entities();
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
