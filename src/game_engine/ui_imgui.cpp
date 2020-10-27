
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

void w_imgui_result::operator=( e_im_result res )
{
	result = res;
}

bool w_imgui_result::was_left_clicked()
{
	return ( result == im_result::left_clicked );
}

bool w_imgui_result::was_right_clicked()
{
	return ( result == im_result::right_clicked );
}

void w_imgui::set_origin( const w_vec2& origin )
{
	this->origin = origin;
}

void w_imgui::clear_origin()
{
	origin = w_vec2::zero;
}

// ----------------------------------------------------------------------------

void w_imgui::reset()
{
	im_automatic_id = 0;
	containing_layer_is_topmost = false;
}

w_imgui* w_imgui::begin_push_button()
{
	control.~w_imgui_control();
	new( &control ) w_imgui_control();

	control.is_active = true;

	return this;
}

w_imgui* w_imgui::begin_panel()
{
	control.~w_imgui_control();
	new( &control ) w_imgui_control();

	control.is_active = false;

	return this;
}

w_imgui* w_imgui::set_label( const std::string& label )
{
	control.label = label;
	return this;
}

w_imgui* w_imgui::set_slice_def( a_9slice_def* slice_def )
{
	control.style.slice_def = slice_def;
	return this;
}

w_imgui* w_imgui::set_subtexture( a_subtexture* subtexture )
{
	control.style.subtexture = subtexture;
	return this;
}

w_imgui* w_imgui::set_rect( w_rect rc )
{
	control.rc = rc;
	return this;
}

w_imgui_result* w_imgui::end()
{
	if( control.is_active )
	{
		return active();
	}

	return passive();
}

//w_imgui_result w_imgui::panel( w_rect rc, w_ui_style_panel* style )
//{
//	w_ui_style_panel default_style;
//
//	if( !style )
//	{
//		style = &default_style;
//	}
//
//	return passive( "", rc, style );
//}

w_imgui_result* w_imgui::active()
{
	last_rect = control.rc;

	im_automatic_id++;

	control.rc.x += origin.x;
	control.rc.y += origin.y;

	if( containing_layer_is_topmost )
	{
		control.result = control.style.get_im_state( im_automatic_id, control.rc );
	}

	control.style.draw( control.label, control.rc, hover_id == im_automatic_id, hot_id == im_automatic_id );

	return &control.result;
}

w_imgui_result* w_imgui::passive()
{
	last_rect = control.rc;

	control.rc.x += origin.x;
	control.rc.y += origin.y;

	control.style.draw( control.label, control.rc, false, false );

	return &control.result;
}
