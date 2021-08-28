
#include "master_pch.h"
#include "master_header.h"

namespace war
{

UI_Control_Data* UI_Callback::get_data( hash tag )
{
	return &default_data;
}

Texture_Asset* UI_Callback::get_texture_for_checkbox( hash tag )
{
	auto checked = get_data( tag )->bool_value();

	return
		checked
		? g_engine->find_asset<Texture_Asset>( "ui_check_on" )
		: g_engine->find_asset<Texture_Asset>( "ui_check_off" );
}

Texture_Asset* UI_Callback::get_texture_for_radio( hash tag )
{
	auto idx = get_data( tag )->int_value();

	return
		( idx == g_ui->current_control->idx )
		? g_engine->find_asset<Texture_Asset>( "ui_radio_on" )
		: g_engine->find_asset<Texture_Asset>( "ui_radio_off" );
}

void UI_Callback::on_control_left_clicked( hash tag, const UI_Result& result )
{
	auto control_data = get_data( tag );

	switch( g_ui->current_control->type )
	{
		case ui_control_type::check:
		{
			control_data->set_bool_value( !control_data->bool_value() );
			on_value_changed( tag );
			break;
		}

		case ui_control_type::radio:
		{
			control_data->set_int_value( g_ui->current_control->idx );
			on_value_changed( tag );
			break;
		}

		case ui_control_type::slider:
		{
			on_motion( tag, result );
			break;
		}
	}
}

void UI_Callback::on_motion( hash tag, const UI_Result& result )
{
	auto control_data = get_data( tag );

	switch( g_ui->current_control->type )
	{
		case ui_control_type::slider:
		{
			auto value = result.click_pct.x;

			if( g_ui->current_control->interval > 0 )
			{
				// nudging the pct value before clamping it allows you to click
				// just before or just after an interval position and have it
				// snap into place correctly.

				auto pct_nudged = result.click_pct.x + ( g_ui->current_control->interval / 2.f );
				value = pct_nudged - glm::mod( pct_nudged, g_ui->current_control->interval );
			}

			control_data->set_float_value_internal( value );
			on_value_changed( tag );
			break;
		}
	}
}

size_t UI_Callback::get_item_count( hash tag )
{
	return 0;
}

std::string_view UI_Callback::get_item_for_idx( hash tag, int idx )
{
	return "n/a";
}

void UI_Callback::on_value_changed( hash tag )
{
}

bool UI_Callback::validate_value_change( hash tag, UI_Control_Data* old_value, UI_Control_Data* new_value )
{
	if( g_ui->focused.type == ui_control_type::text )
	{
		auto old_text_data = static_cast<UI_Text_Control_Data*>( old_value );
		auto new_text_data = static_cast<UI_Text_Control_Data*>( new_value );

		std::string old_str = old_text_data->string_value();
		std::string new_str = new_text_data->string_value();

		if( new_str.empty() )
		{
			// new value is empty, which is always valid
			return true;
		}

		if( old_text_data->max_length > 0 )
		{
			// validate against length

			if( new_text_data->string_value().size() > old_text_data->max_length )
			{
				return false;
			}

			// validate against character list

			if( new_str.size() > old_str.size() )
			{
				if( !old_text_data->valid_char_list.empty() )
				{
					if( old_text_data->valid_char_list.find_first_of( new_str.back() ) == std::string::npos )
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

float UI_Callback::get_control_padding()
{
	return 4.f;
}

// how much space should be left between a control
// and it's neighbors.
//
// i.e. between 2 buttons sitting side by side

Vec2 UI_Callback::get_control_margin( e_ui_control_type control_type )
{
	Vec2 result = { 2.f, 2.f };

	switch( control_type )
	{
		case ui_control_type::slider:
		result.y = 3.f;
		return result;

		case ui_control_type::progress:
		result.y = 4.f;
		return result;

		case ui_control_type::divider:
		result.y = 1.f;
		return result;
	}

	return result;
}

bool UI_Callback::on_input_motion( const Input_Event* evt )
{
	return false;
}

bool UI_Callback::handle_editing_key( const Input_Event* evt )
{
	if( g_ui->focused.type == ui_control_type::text )
	{
		auto control_data = static_cast<UI_Text_Control_Data*>( get_data( g_ui->focused.tag ) );

		// ----------------------------------------------------------------------------
		// standard text editing keys

		switch( evt->input_id )
		{
			case input_id::key_backspace:
			{
				std::string str = control_data->string_value();
				if( !str.empty() and control_data->caret_pos > 0 )
				{
					std::string str_pre_caret = str.substr( 0, control_data->caret_pos - 1 );
					std::string str_post_caret = str.substr( control_data->caret_pos, std::string::npos );

					control_data->set_string_value( str_pre_caret + str_post_caret );
					control_data->caret_pos--;
				}
				return true;
			}

			case input_id::key_delete:
			{
				std::string str = control_data->string_value();
				if( !str.empty() and control_data->caret_pos < str.size() )
				{
					std::string str_pre_caret = str.substr( 0, control_data->caret_pos );
					std::string str_post_caret = str.substr( control_data->caret_pos + 1, std::string::npos );

					control_data->set_string_value( str_pre_caret + str_post_caret );
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
				std::string str = control_data->string_value();

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
				std::string str = control_data->string_value();
				control_data->caret_pos = str.length();
				return true;
			}

			case input_id::key_esc:
			case input_id::key_enter:
			{
				g_ui->focused.tag = hash_none;
				return true;
			}
		}
	}

	return false;
}

bool UI_Callback::on_input_pressed( const Input_Event* evt )
{
	if( g_ui->focused.tag != hash_none )
	{
		if( handle_editing_key( evt ) )
		{
			return true;
		}

		// if a text control has focus, we want to eat all of these events
		// regardless. otherwise each time we press a key the input control
		// gives up it's focused state.
		if( g_ui->focused.type == ui_control_type::text )
		{
			return true;
		}
	}

	return false;
}

bool UI_Callback::on_input_held( const Input_Event* evt )
{
	if( g_ui->focused.tag != hash_none )
	{

		if( handle_editing_key( evt ) )
		{
			return true;
		}

		// if a ui control has focus, we want to eat all of these events regardless
		return true;
	}

	return false;
}

bool UI_Callback::on_input_released( const Input_Event* evt )
{
	return false;
}

bool UI_Callback::on_input_key( const Input_Event* evt )
{
	if( g_ui->focused.tag != hash_none )
	{
		// text controls want keypresses

		if( g_ui->focused.type == ui_control_type::text )
		{
			auto control_data = static_cast<UI_Text_Control_Data*>( get_data( g_ui->focused.tag ) );

			std::string new_str = control_data->string_value();
			new_str.insert( new_str.begin() + control_data->caret_pos, evt->ch );

			auto new_data = UI_Text_Control_Data();
			new_data.set_string_value( new_str );

			if( validate_value_change( g_ui->focused.tag, control_data, &new_data ) )
			{
				control_data->set_string_value( new_data.string_value() );
				control_data->caret_pos++;
			}

			return true;
		}
	}

	return false;
}

}
