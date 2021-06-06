#include "master_pch.h"
#include "master_header.h"

w_imgui_control_data::w_imgui_control_data( bool value )
	: data( value )
{

}

w_imgui_control_data::w_imgui_control_data( std::string value )
	: data( value )
{

}

w_imgui_control_data::w_imgui_control_data( float value )
	: data( value )
{

}

bool w_imgui_control_data::operator==( float value )
{
	return std::get<float>( data ) == value;
}

bool w_imgui_control_data::operator==( std::string value )
{
	return std::get<std::string>( data ) == value;
}

bool w_imgui_control_data::operator==( bool value )
{
	return std::get<bool>( data ) == value;
}

void w_imgui_control_data::operator=( bool value )
{
	data = value;
}

void w_imgui_control_data::operator=( std::string value )
{
	data = value;
}

void w_imgui_control_data::operator=( float value )
{
	data = value;
}
