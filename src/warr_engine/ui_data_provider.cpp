
#include "master_pch.h"
#include "master_header.h"

e_imgui_control_state w_imgui_callback::get_state_for_control( const w_imgui_control& control )
{
	return imgui_control_state::none;
}

a_texture* w_imgui_callback::get_texture_for_checkbox( const w_imgui_control& control )
{
	auto state = get_state_for_control( control );

	if( state == imgui_control_state::checked )
	{
		return a_texture::find( "ui_checkbox_on" );
	}

	return a_texture::find( "ui_checkbox_off" );
}

void w_imgui_callback::was_left_clicked( const w_imgui_control& control )
{
}

void w_imgui_callback::was_right_clicked( const w_imgui_control& control )
{
}

// space to leave between controls within the client area
//
// i.e between a checkbox and it's label

int w_imgui_callback::get_control_padding()
{
	return 4;
}

// how much space should be left between a control
// and it's neighbors.
//
// i.e. between 2 buttons sitting side by side

int w_imgui_callback::get_control_margin()
{
	return 2;
}
