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
