
#include "master_pch.h"
#include "master_header.h"

w_imgui_control_data w_imgui_callback::get_data_for_control(const w_imgui_control& control)
{
	return w_imgui_control_data( 0.0f );
}

a_texture* w_imgui_callback::get_texture_for_checkbox( const w_imgui_control& control )
{
	w_imgui_control_data* control_data = IMGUI->get_control_data( control.tag );
	auto checked = std::get<bool>( control_data ? control_data->data : get_data_for_control( control ).data );

	if( checked )
	{
		return a_texture::find( "ui_checkbox_on" );
	}

	return a_texture::find( "ui_checkbox_off" );
}

void w_imgui_callback::on_left_clicked( const w_imgui_control& control, const w_imgui_result& result )
{
	auto control_data = IMGUI->get_control_data( control.tag );

	switch( control.type )
	{
		case imgui_control_type::check_box:
		{
			control_data->data = !std::get<bool>( control_data->data );
		}
		break;

		case imgui_control_type::slider:
		{
			control_data->data = result.click_pct.x;

			if( control.interval > 0 )
			{
				control_data->data = result.click_pct.x - glm::mod( result.click_pct.x, control.interval );
			}
		}
		break;
	}
}

void w_imgui_callback::on_motion( const w_imgui_control& control, const w_imgui_result& result )
{
	auto control_data = IMGUI->get_control_data( control.tag );

	switch( control.type )
	{
		case imgui_control_type::slider:
		{
			*control_data = result.click_pct.x;

			if( control.interval > 0 )
			{
				*control_data = result.click_pct.x - glm::mod( result.click_pct.x, control.interval );
			}
		}
		break;
	}
}

bool w_imgui_callback::validate_value_change( hash tag, w_imgui_control_data& old_value, w_imgui_control_data& new_value )
{
	if( old_value.control_type == imgui_control_type::edit_box )
	{
		std::string new_str = std::get<std::string>( new_value.data );
		std::string old_str = std::get<std::string>( old_value.data );

		if( new_str.empty() )
		{
			// new value is empty, which is always valid
			return true;
		}

		if( old_value.max_length > 0 )
		{
			// validate against length

			if( std::get<std::string>( new_value.data ).size() > old_value.max_length )
			{
				return false;
			}

			// validate against character list

			if( new_str.size() > old_str.size() )
			{
				if( !old_value.valid_char_list.empty() )
				{
					if( !w_string_util::contains_char( old_value.valid_char_list, new_str.back() ) )
					{
						return false;
					}
				}
			}
		}
	}

	return true;
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

		case imgui_control_type::spacer:
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

bool w_imgui_callback::handle_editing_key( const w_input_event* evt )
{
	auto control_data = IMGUI->get_control_data( UI->tag_focus );
	assert( control_data );	// a control has focus but isn't in the tag/data map?

	if( control_data->control_type == imgui_control_type::edit_box )
	{
		// ----------------------------------------------------------------------------
		// standard text editing keys

		switch( evt->input_id )
		{
			case input_id::key_backspace:
			{
				std::string str = std::get<std::string>( control_data->data );
				if( !str.empty() && control_data->caret_pos > 0 )
				{
					std::string str_pre_caret = str.substr( 0, control_data->caret_pos - 1 );
					std::string str_post_caret = str.substr( control_data->caret_pos, std::string::npos );

					*control_data = str_pre_caret + str_post_caret;;
					control_data->caret_pos--;
				}
				return true;
			}

			case input_id::key_delete:
			{
				std::string str = std::get<std::string>( control_data->data );
				if( !str.empty() && control_data->caret_pos < str.size() )
				{
					std::string str_pre_caret = str.substr( 0, control_data->caret_pos );
					std::string str_post_caret = str.substr( control_data->caret_pos + 1, std::string::npos );

					*control_data = str_pre_caret + str_post_caret;
				}
				return true;
			}

			case input_id::key_left:
			{
				if( control_data->caret_pos > 0 )
				{
					control_data->caret_pos--;
				}
				return true;
			}

			case input_id::key_right:
			{
				std::string str = std::get<std::string>( control_data->data );

				control_data->caret_pos++;
				control_data->caret_pos = glm::min<size_t>( str.size(), control_data->caret_pos );
				return true;
			}

			case input_id::key_home:
			{
				control_data->caret_pos = 0;
				return true;
			}

			case input_id::key_end:
			{
				std::string str = std::get<std::string>( control_data->data );
				control_data->caret_pos = str.length();
				return true;
			}

			case input_id::key_esc:
			case input_id::key_enter:
			{
				UI->tag_focus = hash_none;
				return true;
			}
		}
	}

	return false;
}

bool w_imgui_callback::on_input_pressed( const w_input_event* evt )
{
	if( UI->tag_focus != hash_none )
	{
		auto control_data = IMGUI->get_control_data( UI->tag_focus );

		if( control_data )
		{
			if( handle_editing_key( evt ) )
			{
				return true;
			}

			// if a ui control has focus, we want to eat all of these events regardless
			return true;
		}
	}

	return false;
}

bool w_imgui_callback::on_input_held( const w_input_event* evt )
{
	if( UI->tag_focus != hash_none )
	{
		auto control_data = IMGUI->get_control_data( UI->tag_focus );

		if( control_data )
		{
			if( handle_editing_key( evt ) )
			{
				return true;
			}

			// if a ui control has focus, we want to eat all of these events regardless
			return true;
		}
	}

	return false;
}

bool w_imgui_callback::on_input_released( const w_input_event* evt )
{
	return false;
}

bool w_imgui_callback::on_input_key( const w_input_event* evt )
{
	if( UI->tag_focus != hash_none )
	{
		auto control_data = IMGUI->get_control_data( UI->tag_focus );
		assert( control_data );	// a control has focus but isn't in the tag/data map?

		// ----------------------------------------------------------------------------
		// text controls want keypresses

		if( control_data->control_type == imgui_control_type::edit_box )
		{
			std::string new_str = std::get<std::string>( control_data->data );
			new_str.insert( new_str.begin() + control_data->caret_pos, evt->ch );
			auto new_data = w_imgui_control_data( new_str );

			if( validate_value_change( UI->tag_focus, *control_data, new_data ) )
			{
				control_data->data = new_data.data;
				control_data->caret_pos++;
			}
		}
	}

	return false;
}
