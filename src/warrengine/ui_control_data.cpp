#include "master_pch.h"
#include "master_header.h"

namespace war
{

// ----------------------------------------------------------------------------

bool UI_Control_Data::is_expanded()
{
	return false;
}

void UI_Control_Data::set_expanded( bool expanded )
{
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

float UI_Control_Data::float_value()
{
	return _float_value;
}

void UI_Control_Data::set_float_value( float value )
{
	_float_value = value;
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

float UI_Control_Data::float_value_internal()
{
	return _float_value;
}

void UI_Control_Data::set_float_value_internal( float value )
{
	_float_value = value;
}

int UI_Control_Data::int_value()
{
	return _int_value;
}

void UI_Control_Data::set_int_value( int value )
{
	_int_value = value;
}

// ----------------------------------------------------------------------------
// slider controls automatically convert back and forth between their internal
// perctanges and the values in their range slider.

float UI_Slider_Control_Data::float_value()
{
	return slider_range.get_value_at_pct( UI_Control_Data::float_value() );
}

void UI_Slider_Control_Data::set_float_value( float value )
{
	return UI_Control_Data::set_float_value( slider_range.get_pct_from_value( value ) );
}

// ----------------------------------------------------------------------------

bool UI_Dropdown_Control_Data::is_expanded()
{
	return _expanded;
}

void UI_Dropdown_Control_Data::set_expanded( bool expanded )
{
	_expanded = expanded;
}

}