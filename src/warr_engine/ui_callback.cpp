
#include "master_pch.h"
#include "master_header.h"

w_imgui_control_data w_imgui_callback::get_data_for_control(const w_imgui_control& control)
{
	return w_imgui_control_data( 0.0f );
}

a_texture* w_imgui_callback::get_texture_for_checkbox( const w_imgui_control& control )
{
	auto checked = std::get<bool>( get_data_for_control( control ) );

	if( checked )
	{
		return a_texture::find( "ui_checkbox_on" );
	}

	return a_texture::find( "ui_checkbox_off" );
}

void w_imgui_callback::on_left_clicked( const w_imgui_control& control, const w_imgui_result& result )
{
}

void w_imgui_callback::on_motion( const w_imgui_control& control, const w_imgui_result& result )
{
}

// space to leave between controls within the client area
//
// i.e between a checkbox and it's label
// #theme

float w_imgui_callback::get_control_padding()
{
	return 4.0f;
}

// how much space should be left between a control
// and it's neighbors.
//
// i.e. between 2 buttons sitting side by side
// #theme

float w_imgui_callback::get_control_margin()
{
	return 2.0f;
}

// #theme
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

// #theme

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
		{
			return 12.0f;
		}

		case imgui_control_type::slider:
		{
			return 6.0f;
		}

		case imgui_control_type::divider:
		{
			return 4.0f;
		}

		case imgui_control_type::edit_box:
		{
			return 16.0f;
		}
	}

	return 100.0f;
}

bool w_imgui_callback::on_input_motion( const w_input_event* evt )
{
	return false;
}

bool w_imgui_callback::on_input_pressed( const w_input_event* evt )
{
	return false;
}

bool w_imgui_callback::on_input_held( const w_input_event* evt )
{
	return false;
}

bool w_imgui_callback::on_input_released( const w_input_event* evt )
{
	return false;
}
