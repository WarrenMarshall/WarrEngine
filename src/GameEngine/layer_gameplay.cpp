
#include "master_pch.h"
#include "master_header.h"

// ------------------------------------------------------------------------------

void layer_gameplay::push()
{
	background_gradient = static_cast<a_gradient*>( engine->asset_cache->find( "background_gradient" ) );

	engine->input_mgr->add_listener( this );
}

void layer_gameplay::pop()
{
	engine->input_mgr->remove_listener( this );
}

void layer_gameplay::update()
{
	w_layer::update();

}

void layer_gameplay::draw()
{
	w_layer::draw();
	game->draw_entities();

	engine->opengl->push_matrix();
	engine->opengl->translate( w_vec3( -v_window_hw, -v_window_hh, -100 ) );
	engine->render->draw( background_gradient, w_color( 1.0, 1.0, 1.0, 0.15f ), w_vec2( v_window_w, v_window_h ) );
	engine->opengl->pop_matrix();

	engine->render->draw_string( game->font, w_vec3( -v_window_hw + 8, v_window_hh - 8, 1 ), "SHIELD MAIDEN", e_align::left, W_COLOR_WHITE );
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
				log_msg( "thrust!" );
			}
			break;
		}

		//	case e_input_id::keyboard_left:
		//		game->player->angle = 90;
		//		break;
		//	case e_input_id::controller_button_dpad_up:
		//	case e_input_id::keyboard_up:
		//		game->player->angle = 0;
		//		break;
		//	case e_input_id::controller_button_dpad_right:
		//	case e_input_id::keyboard_right:
		//		game->player->angle = 270;
		//		break;
		//	case e_input_id::controller_button_dpad_down:
		//	case e_input_id::keyboard_down:
		//		game->player->angle = 180;
		//		break;
		//}
	}
	else if( event == e_event_id::input_motion )
	{
		//log_msg( "%s : %f, %f", __FUNCTION__, evt->data.mouse.xdelta, evt->data.mouse.ydelta );

		game->player->angle -= evt->data.xdelta;

		//w_vec3 new_pos = game->player->pos;
		//new_pos.x = evt->pos.x - v_window_hw;
		//new_pos.y = (v_window_h - evt->pos.y) - v_window_hh;

		//game->player->set_transform( new_pos, 0, 1.0f );
	}
}
