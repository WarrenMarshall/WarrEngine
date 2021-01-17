
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

void w_imgui_result::operator=( const e_im_result res )
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

// ----------------------------------------------------------------------------

void w_imgui::reset()
{
	im_automatic_id = 0;
	containing_layer_is_topmost = false;
	last_control = std::nullopt;
	tagged_controls.clear();
}

// locates the control that matches "tag" and sets up the imgui
// to pretend that was the last control we drew.
//
// this is for finding controls that were created previously
// that you now need to reference for positioning or parenting.

w_imgui* w_imgui::set_last_control_from_tag( hash tag )
{
	_set_as_last_control( find_control( tag ) );

	return this;
}

w_imgui_control w_imgui::find_control( hash tag )
{
	auto iter = tagged_controls.find( tag );

	if( iter == tagged_controls.end() )
	{
		log_error( "{} : tagged control not found", tag );
	}

	return iter->second;
}

w_imgui* w_imgui::clear_last_control()
{
	last_control = std::nullopt;
	return this;
}

void w_imgui::set_current_callback_from_current_layer()
{
	current_callback = &default_callback;
	if( IMGUI->containing_layer_is_topmost )
	{
		current_callback = LAYER->get_imgui_callback();
	}
}

w_imgui* w_imgui::init_panel( hash tag )
{
	set_current_callback_from_current_layer();

	current_control = {};
	current_control.type = imgui_control_type::panel;
	current_control.tag = tag;
	current_control.is_active = false;
	current_control.slice_def = a_9slice_def::find( "simple_ui_panel" );
	current_control.text_align = align::left | align::top;

	set_size( { w_sz::def, w_sz::def } );

	return this;
}

w_imgui* w_imgui::init_push_button( hash tag )
{
	set_current_callback_from_current_layer();

	current_control = {};
	current_control.type = imgui_control_type::push_button;
	current_control.tag = tag;
	current_control.is_active = true;
	current_control.slice_def = a_9slice_def::find( "simple_ui_push_button" );
	current_control.text_align = align::centered;

	set_size( { w_sz::def, w_sz::def } );

	return this;
}

w_imgui* w_imgui::init_checkbox( hash tag )
{
	set_current_callback_from_current_layer();

	current_control = {};
	current_control.type = imgui_control_type::check_box;
	current_control.tag = tag;
	current_control.is_active = true;
	current_control.text_align = align::left | align::vcenter;

	set_size( { w_sz::def, w_sz::def } );

	return this;
}

w_imgui* w_imgui::init_divider( hash tag )
{
	set_current_callback_from_current_layer();

	current_control = {};
	current_control.type = imgui_control_type::divider;
	current_control.tag = tag;
	current_control.is_active = false;
	current_control.slice_def = a_9slice_def::find( "simple_ui_divider" );

	set_size( { w_sz::def, w_sz::def } );

	return this;
}

w_imgui* w_imgui::init_label( hash tag )
{
	set_current_callback_from_current_layer();

	current_control = {};
	current_control.type = imgui_control_type::label;
	current_control.tag = tag;
	current_control.is_active = false;
	current_control.text_align = align::centered;

	set_size( { w_sz::def, w_sz::def } );

	return this;
}

w_imgui* w_imgui::init_slider( hash tag )
{
	set_current_callback_from_current_layer();

	current_control = {};
	current_control.type = imgui_control_type::slider;
	current_control.tag = tag;
	current_control.is_active = true;
	current_control.text_align = align::left | align::vcenter;

	set_size( { w_sz::def, w_sz::def } );

	return this;
}

w_imgui* w_imgui::set_text( const std::string& text )
{
	current_control.text = text;
	return this;
}

w_imgui* w_imgui::set_text_align( e_align align )
{
	current_control.text_align = align;
	return this;
}

w_imgui* w_imgui::set_slice_def( const std::string& tag )
{
	current_control.slice_def = a_9slice_def::find( tag );
	compute_clientrect_from_rect();

	return this;
}

w_imgui* w_imgui::set_position( const w_pos& pos )
{
	current_control.rc_win = last_control->rc_win;

	current_control.rc_win.x = pos.x;
	current_control.rc_win.y = pos.y;

	compute_clientrect_from_rect();

	return this;
}

w_imgui* w_imgui::set_position( e_imgui_flow flow )
{
	current_control.rc_win.x = last_control->rc_win.x;
	current_control.rc_win.y = last_control->rc_win.y;

	if( flow & imgui_flow::right )
	{
		current_control.rc_win.x = flow_right.x;
		current_control.rc_win.y = flow_right.y;
	}

	if( flow & imgui_flow::down )
	{
		current_control.rc_win.x = flow_down.x;
		current_control.rc_win.y = flow_down.y;
	}

	if( flow & imgui_flow::last_crc_topleft )
	{
		current_control.rc_win.x = last_control->rc_client.x;
		current_control.rc_win.y = last_control->rc_client.y;
	}

	compute_clientrect_from_rect();

	return this;
}

w_imgui* w_imgui::set_size( const w_sz& sz )
{
	if( sz.w != w_sz::ignore )
	{
		current_control.rc_win.w =
			( sz.w == w_sz::def )
			? current_callback->get_default_width( current_control )
			: sz.w;
	}
	if( sz.h != w_sz::ignore )
	{
		current_control.rc_win.h =
			( sz.h == w_sz::def )
			? current_callback->get_default_height( current_control )
			: sz.h;
	}

	compute_clientrect_from_rect();

	return this;
}

// copies the overall control rect to the client rect, and then makes adjustments
// to compensate for graphical frames and whatever else would affect the client area.

void w_imgui::compute_clientrect_from_rect()
{
	current_control.rc_client = current_control.rc_win;

	if( current_control.slice_def )
	{
		current_control.rc_client.x += current_control.slice_def->patches[ slicedef_patch::P_00 ]->rc.w;
		current_control.rc_client.y += current_control.slice_def->patches[ slicedef_patch::P_00 ]->rc.h;

		current_control.rc_client.w -= current_control.slice_def->patches[ slicedef_patch::P_00 ]->rc.w + current_control.slice_def->patches[ slicedef_patch::P_22 ]->rc.w;
		current_control.rc_client.h -= current_control.slice_def->patches[ slicedef_patch::P_00 ]->rc.h + current_control.slice_def->patches[ slicedef_patch::P_22 ]->rc.h;
	}
}

w_imgui_result* w_imgui::finalize()
{
	if( current_control.is_active )
	{
		_active();
	}
	else
	{
		_passive();
	}

	if( current_control.tag )
	{
		tagged_controls.insert( std::pair( current_control.tag, current_control ) );
	}

	_set_as_last_control( current_control );

	if( result.was_left_clicked() )
	{
		current_callback->was_left_clicked( current_control );
	}
	if( result.was_right_clicked() )
	{
		current_callback->was_right_clicked( current_control );
	}

	return &result;
}

void w_imgui::_active()
{
	result = {};
	im_automatic_id++;

	if( containing_layer_is_topmost )
	{
		result = _update_im_state( im_automatic_id, current_control.rc_win );
	}

	_draw( current_control, hover_id == im_automatic_id, hot_id == im_automatic_id );
}

void w_imgui::_passive()
{
	result = {};

	_draw( current_control, false, false );
}

e_im_result w_imgui::_update_im_state( int id, const w_rect& rc_win )
{
	assert( rc_win.w );
	assert( rc_win.h );

	e_im_result imresult = im_result::none;

	/*
		 reduce the size of the hit rectangle. this gives more breathing room
		 when mousing over tightly packed UI elements and stops the user
		 from being able to highlight/click more than one at a time.
	*/

	w_rect rc_hit = rc_win - w_rect( 0.0f, 0.0f, 1.0f, 1.0f );

	e_button_state bs_left = engine->input->get_button_state( input_id::mouse_button_left );
	bool mouse_is_inside = UI->is_mouse_inside( rc_hit );

	if( mouse_is_inside )
	{
		if( bs_left == button_state::up || ( bs_left == button_state::held && IMGUI->hot_id == id ) )
		{
			IMGUI->hover_id = id;
		}
		else if( bs_left == button_state::pressed )
		{
			IMGUI->hot_id = id;
		}
		else if( bs_left == button_state::released )
		{
			if( IMGUI->hot_id == id && IMGUI->hover_id == id )
			{
				imresult |= im_result::left_clicked;
			}
			IMGUI->hover_id = IMGUI->hot_id = -1;
		}
	}
	else
	{
		if( IMGUI->hover_id == id )
		{
			IMGUI->hover_id = -1;
		}

		if( bs_left == button_state::released && IMGUI->hot_id == id )
		{
			IMGUI->hot_id = -1;
		}
	}

	if( IMGUI->hover_id == id )
	{
		imresult |= im_result::hovered;
	}
	if( IMGUI->hot_id == id )
	{
		imresult |= im_result::hot;
	}

	return imresult;
}

void w_imgui::_draw( const w_imgui_control& control, bool being_hovered, bool being_clicked )
{
	w_vec2 clicked_offset = _get_click_offset( being_hovered, being_clicked );

	w_rect rc_win_offset = control.rc_win;
	rc_win_offset.x += clicked_offset.x;
	rc_win_offset.y += clicked_offset.y;

	w_rect rc_client_offset = control.rc_client;
	rc_client_offset.x += clicked_offset.x;
	rc_client_offset.y += clicked_offset.y;

	RENDER->begin();

	switch( control.type )
	{
		case imgui_control_type::panel:
		{
			_draw_slice_def( control, rc_win_offset, being_hovered, being_clicked );
		}
		break;

		case imgui_control_type::push_button:
		{
			_draw_slice_def( control, rc_win_offset, being_hovered, being_clicked );
			_draw_text( control, rc_client_offset, being_hovered, being_clicked );
		}
		break;

		case imgui_control_type::check_box:
		{
			a_texture* texture = current_callback->get_texture_for_checkbox( control );

			_draw_slice_def( control, rc_win_offset, being_hovered, being_clicked );

			w_rect rc_texture =
				w_rect(
					rc_client_offset.x, rc_client_offset.y,
					texture->rc.w,
					rc_client_offset.h
				);
			rc_client_offset =
				w_rect( rc_client_offset.x + current_callback->get_control_padding() + texture->rc.w, rc_client_offset.y,
						rc_client_offset.w - current_callback->get_control_padding() - texture->rc.w, rc_client_offset.h
				);

			_draw_texture( control, rc_texture, texture, being_hovered, being_clicked );
			_draw_text( control, rc_client_offset, being_hovered, being_clicked );
		}
		break;

		case imgui_control_type::label:
		{
			_draw_slice_def( control, rc_win_offset, false, false );
			_draw_text( control, rc_client_offset, false, false );
		}
		break;

		case imgui_control_type::divider:
		{
			_draw_slice_def( control, rc_win_offset, false, false );
		}
		break;
	}

	RENDER->end();
}

void w_imgui::_draw_slice_def( const w_imgui_control& control, const w_rect& rc_win, bool being_hovered, bool being_clicked )
{
	if( control.slice_def )
	{
		RENDER->push_depth_nudge()
			->push_rgb( _get_adjusted_color( w_color::pal( 1 ), being_hovered, being_clicked ) )
			->draw_sliced( control.slice_def, rc_win );
	}
}

void w_imgui::_draw_texture( const w_imgui_control& control, const w_rect& rc, const a_texture* texture, bool being_hovered, bool being_clicked )
{
	RENDER->push_depth_nudge()
		->push_rgb( _get_adjusted_color( w_color::pal( 2 ), being_hovered, being_clicked ) )
		->draw_sprite( texture, rc.midpoint() );
}

void w_imgui::_draw_text( const w_imgui_control& control, const w_rect& rc_client, bool being_hovered, bool being_clicked )
{
	if( control.text.length() )
	{
		w_pos pos = rc_client.get_position_from_alignment( control.text_align );

		RENDER->push_depth_nudge()
			->push_rgb( _get_adjusted_color( w_color::pal( 2 ), being_hovered, being_clicked ) )
			->push_align( control.text_align )
			->draw_string( control.text, pos );
	}
}

// takes a base color and modifies it based on the state of the UI

w_color w_imgui::_get_adjusted_color( const w_color& base_color, bool being_hovered, bool being_clicked )
{
	w_color color = base_color;

	if( being_clicked )
	{
		w_color::scale( color, 1.75f );
	}
	else if( being_hovered )
	{
		w_color::scale( color, 1.25f );
	}

	return color;
}

void w_imgui::_set_as_last_control( w_imgui_control control )
{
	last_control = control;

	flow_right = { last_control->rc_win.x + last_control->rc_win.w + current_callback->get_control_margin(), last_control->rc_win.y };
	flow_down = { last_control->rc_win.x, last_control->rc_win.y + last_control->rc_win.h + current_callback->get_control_margin() };
}

// a control with the mouse button held down on it will offset slightly

w_offset w_imgui::_get_click_offset( bool being_hovered, bool being_clicked )
{
	if( being_hovered && being_clicked )
	{
		return w_offset( 1, 1 );
	}

	return w_offset( 0, 0 );
}
