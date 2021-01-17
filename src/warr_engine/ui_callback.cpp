
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
	log_warning( "left clicked not handled!" );
}

void w_imgui_callback::was_right_clicked( const w_imgui_control& control )
{
	log_warning( "right clicked not handled!" );
}

// space to leave between controls within the client area
//
// i.e between a checkbox and it's label

float w_imgui_callback::get_control_padding()
{
	return 4.0f;
}

// how much space should be left between a control
// and it's neighbors.
//
// i.e. between 2 buttons sitting side by side

float w_imgui_callback::get_control_margin()
{
	return 2.0f;
}

float w_imgui_callback::get_default_width( const w_imgui_control& control )
{
	return get_default_width( control.type );
}

float w_imgui_callback::get_default_width( e_imgui_control_type control_type )
{
	switch( control_type )
	{
		case imgui_control_type::slider:
		{
			return 100.0f;
		}
	}

	return 120.0f;
}

float w_imgui_callback::get_default_height( const w_imgui_control& control )
{
	return get_default_height( control.type );
}

float w_imgui_callback::get_default_height( e_imgui_control_type control_type )
{
	switch( control_type )
	{
		case imgui_control_type::push_button:
		{
			return 24.0f;
		}

		case imgui_control_type::check_box:
		case imgui_control_type::label:
		case imgui_control_type::slider:
		{
			return 12.0f;
		}

		case imgui_control_type::divider:
		{
			return 4.0f;
		}
	}

	return 100.0f;
}
