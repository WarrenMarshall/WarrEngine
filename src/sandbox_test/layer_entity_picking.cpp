
#include "app_header.h"

// ----------------------------------------------------------------------------

void layer_entity_picking_callback::on_left_clicked( const w_imgui_control& control, const w_imgui_result& result )
{
	w_imgui_callback::on_left_clicked( control, result );

// 	switch( control.tag )
// 	{
// 		case H( "push_button_01" ):
// 		{
// 			UI->show_msg_box( "You clicked the button!" );
// 		}
// 		break;
// 	}
}

void layer_entity_picking_callback::on_motion( const w_imgui_control& control, const w_imgui_result& result )
{
	w_imgui_callback::on_motion( control, result );

	auto layer = static_cast<layer_entity_picking*>( IMGUI->current_layer );

// 	switch( control.tag )
// 	{
// 		case H( "slider_01" ):
// 		{
// 		}
// 		break;
// 	}
}

// ----------------------------------------------------------------------------

layer_entity_picking::layer_entity_picking()
{
	draws_completely_solid = true;
}

void layer_entity_picking::push()
{
	engine->window->set_mouse_mode( mouse_mode::os );
}

void layer_entity_picking::draw()
{
	RS->color = w_color::dark_teal;
	RENDER->draw_tiled( a_texture::find( "engine_tile_background_stripe" ), w_rect( 0.0f, 0.0f, ui_window_w, ui_window_h ) );

	RENDER->push();
	RS->align = align::centered;
	RS->scale = 2.0f;
	RS->color = w_color::white;
	RENDER->draw_string( "Entity Picking", { ui_window_w / 2.0f, 16.0f } );
	RENDER->pop();

	w_layer::draw();
}

void layer_entity_picking::draw_ui()
{
	w_layer::draw_ui();
}

w_imgui_callback* layer_entity_picking::get_imgui_callback()
{
	return &imgui_callback;
}
