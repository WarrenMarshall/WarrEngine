#include "master_pch.h"
#include "master_header.h"

namespace war
{

UI_Control_Data::UI_Control_Data()
	: type( e_ui_control_type::none )
{
}

UI_Control_Data::UI_Control_Data( e_ui_control_type type )
	: type( type )
{
}

UI_Control_Data::~UI_Control_Data()
{
}

bool UI_Control_Data::is_expanded()
{
	switch( type )
	{
		case e_ui_control_type::dropdown:
		{
			return dropdown.expanded;
		}
	}

	return false;
}

void UI_Control_Data::set_expanded( bool expanded )
{
	switch( type )
	{
		case e_ui_control_type::dropdown:
		{
			dropdown.expanded = expanded;
		}
		break;
	}
}

std::string UI_Control_Data::string_value()
{
	return _string_value;
}

void UI_Control_Data::set_string_value( std::string value )
{
	_string_value = value;
}

bool UI_Control_Data::bool_value()
{
	return _bool_value;
}

void UI_Control_Data::set_bool_value( bool value )
{
	_bool_value = value;
}

float_t UI_Control_Data::float_value()
{
	switch( type )
	{
		case e_ui_control_type::slider:
		{
			// slider controls automatically convert back and forth between their internal
			// perctanges and the values in their range slider.

			return slider.range.get_value_at_pct( _float_value );
		}
	}

	return _float_value;
}

void UI_Control_Data::set_float_value( float_t value )
{
	switch( type )
	{
		case e_ui_control_type::slider:
		{
			// slider controls automatically convert back and forth between their internal
			// perctanges and the values in their range slider.

			_float_value = slider.range.get_pct_from_value( value );
		}
		break;

		default:
		{
			_float_value = value;
		}
	}
}

// the _internal functions work with the literal 0-1 range that some controls
// represent (like sliders). these values can be transformed into different
// ranges for UI purposes but the internals remain the same 0-1 range.
//
// they're a level of abstraction that make working with weird ranges for
// sliders pretty painless.
//
// if you really do want that 0-1 range, use the _internal functions. otherwise,
// call the standard functions for getting/setting the float.

float_t UI_Control_Data::float_value_internal()
{
	return _float_value;
}

void UI_Control_Data::set_float_value_internal( float_t value )
{
	_float_value = value;
}

int32_t UI_Control_Data::int_value()
{
	return _int_value;
}

void UI_Control_Data::set_int_value( int32_t value )
{
	_int_value = value;
}

}