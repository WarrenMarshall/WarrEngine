#include "master_pch.h"
#include "master_header.h"

namespace war
{

// ----------------------------------------------------------------------------

bool ui_control_data::is_expanded()
{
	return false;
}

void ui_control_data::set_expanded( bool expanded )
{
}

std::string ui_control_data::string_value()
{
	return _string_value;
}

void ui_control_data::set_string_value( std::string value )
{
	_string_value = value;
}

bool ui_control_data::bool_value()
{
	return _bool_value;
}

void ui_control_data::set_bool_value( bool value )
{
	_bool_value = value;
}

float ui_control_data::float_value()
{
	return _float_value;
}

void ui_control_data::set_float_value( float value )
{
	_float_value = value;
}

// the _internal functions work with the literal 0-1 range that some controls
// represent (like sliders). these values can be transformed into different
// ranges for UI purposes but the internals remain the same 0-1 range.
//
// so if you want that 0-1 range, use the _internal functions. otherwise, call
// the standard functions for getting/setting the float.

float ui_control_data::float_value_internal()
{
	return _float_value;
}

void ui_control_data::set_float_value_internal( float value )
{
	_float_value = value;
}

int ui_control_data::int_value()
{
	return _int_value;
}

void ui_control_data::set_int_value( int value )
{
	_int_value = value;
}

// ----------------------------------------------------------------------------
// slider controls automatically convert back and forth between their internal
// perctanges and the values in their range slider.

float ui_slider_control_data::float_value()
{
	return slider_range.get_value_at_pct( ui_control_data::float_value() );
}

void ui_slider_control_data::set_float_value( float value )
{
	return ui_control_data::set_float_value( slider_range.get_pct_from_value( value ) );
}

// ----------------------------------------------------------------------------

bool ui_dropdown_control_data::is_expanded()
{
	return _expanded;
}

void ui_dropdown_control_data::set_expanded( bool expanded )
{
	_expanded = expanded;
}

}