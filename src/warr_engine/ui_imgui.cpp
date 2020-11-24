
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

w_imgui* w_imgui::set_last_control_from_tag( const char* tag )
{
	_set_as_last_control( find_control( tag ) );

	return this;
}

w_imgui_control w_imgui::find_control( const char* tag )
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

w_imgui* w_imgui::init_push_button( const char* tag )
{
	current_control = {};
	current_control.type = imgui_control_type::push_button;
	current_control.tag = tag;
	current_control.is_active = true;

	return this;
}

w_imgui* w_imgui::init_checkbox( const char* tag )
{
	current_control = {};
	current_control.type = imgui_control_type::check_box;
	current_control.tag = tag;
	current_control.is_active = true;

	return this;
}

w_imgui* w_imgui::init_panel( const char* tag )
{
	current_control = {};
	current_control.type = imgui_control_type::panel;
	current_control.tag = tag;
	current_control.is_active = false;

	return this;
}

w_imgui* w_imgui::set_label( const std::string& label, e_align align )
{
	current_control.label = label;
	current_control.label_align = align;
	return this;
}

w_imgui* w_imgui::set_slice_def( a_9slice_def* slice_def )
{
	current_control.slice_def = slice_def;
	return this;
}

w_imgui* w_imgui::set_subtexture( e_imgui_control_state state, a_subtexture* subtexture )
{
	current_control.subtextures[ state ] = subtexture;

	return this;
}

w_imgui* w_imgui::set_subtexture_align( e_align align )
{
	current_control.subtexture_align = align;
	return this;
}

w_imgui* w_imgui::set_rect( w_rect rc )
{
	current_control.rc = rc;
	calc_client_rect();

	return this;
}

w_imgui* w_imgui::set_rect( e_imgui_flow flow )
{
	return set_rect( flow, { last_control->rc.w, last_control->rc.h } );
}

w_imgui* w_imgui::set_rect( e_imgui_flow flow, w_sz sz )
{
	current_control.rc = last_control->rc;

	if( flow & imgui_flow::right )
	{
		current_control.rc = { flow_right.x, flow_right.y, sz.w, sz.h };
	}

	if( flow & imgui_flow::down )
	{
		current_control.rc = { flow_down.x, flow_down.y, sz.w, sz.h };
	}

	if( flow & imgui_flow::last_crc_topleft )
	{
		current_control.rc = { last_control->crc.x, last_control->crc.y, sz.w, sz.h };
	}

	if( flow & imgui_flow::vcenter )
	{
		current_control.rc.y = ( last_control->rc.y + ( last_control->rc.h / 2.0f ) ) - ( sz.h / 2.0f );
	}

	calc_client_rect();

	return this;
}

// copies the overall control rect to the client rect, and then makes adjustments
// to compensate for graphical frames and whatever else would affect the client area.

void w_imgui::calc_client_rect()
{
	current_control.crc = current_control.rc;

	if( current_control.slice_def )
	{
		current_control.crc.x += current_control.slice_def->patches[ slicedef_patch::P_00 ]->rc_tex.w;
		current_control.crc.y += current_control.slice_def->patches[ slicedef_patch::P_00 ]->rc_tex.h;

		current_control.crc.w -= current_control.slice_def->patches[ slicedef_patch::P_00 ]->rc_tex.w + current_control.slice_def->patches[ slicedef_patch::P_22 ]->rc_tex.w;
		current_control.crc.h -= current_control.slice_def->patches[ slicedef_patch::P_00 ]->rc_tex.h + current_control.slice_def->patches[ slicedef_patch::P_22 ]->rc_tex.h;
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

	if( current_control.tag != nullptr )
	{
		tagged_controls.insert( std::pair( current_control.tag, current_control ) );
	}

	_set_as_last_control( current_control );

	return &result;
}

void w_imgui::_active()
{
	result = {};
	im_automatic_id++;

	if( containing_layer_is_topmost )
	{
		result = _update_im_state( im_automatic_id, current_control.rc );
	}

	_draw( current_control, hover_id == im_automatic_id, hot_id == im_automatic_id );
}

void w_imgui::_passive()
{
	result = {};

	_draw( current_control, false, false );
}

e_im_result w_imgui::_update_im_state( int id, w_rect rc )
{
	assert( rc.w );
	assert( rc.h );

	e_im_result imresult = im_result::none;

	/*
		 reduce the size of the hit rectangle. this gives more breathing room
		 when mousing over tightly packed UI elements and stops the user
		 from being able to highlight/click more than one at a time.
	*/

	rc.w -= 1.0f;
	rc.h -= 1.0f;

	e_button_state bs_left = engine->input->get_button_state( input_id::mouse_button_left );
	bool mouse_is_inside = UI->is_mouse_inside( rc );

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

void w_imgui::_draw( w_imgui_control& control, bool being_hovered, bool being_clicked )
{
	w_imgui_data_provider* data_provider = nullptr;
	if( IMGUI->containing_layer_is_topmost )
	{
		data_provider = engine->layer_mgr->get_top()->ui_data_provider;
	}

	w_vec2 base_offset = control.get_base_offset();
	w_vec2 clicked_offset = _get_click_offset( being_hovered, being_clicked );

	w_rect rc_draw = control.rc;
	rc_draw.x += clicked_offset.x;
	rc_draw.y += clicked_offset.y;

	w_pos label_pos = rc_draw.midpoint();

	// begin

	RENDER
		->begin();

	// slice def

	if( control.slice_def )
	{
		RENDER
			->push_depth_nudge()
			->push_rgb( _get_adjusted_color( w_color::dark_teal, being_hovered, being_clicked ) )
			->draw_sliced( control.slice_def, rc_draw );
	}

	// subtexture

	a_subtexture* subtex = control.subtextures[ data_provider ? data_provider->get_subtexture_idx( &control ) : 0 ];

	if( subtex )
	{
		w_rect subtex_rc = control.crc + clicked_offset;

		if( control.subtexture_align & align::left )
		{
			subtex_rc.w = subtex->get_bounding_rect().w;
			subtex_rc.h = subtex->get_bounding_rect().h;
		}

		RENDER
			->push_rgb( _get_adjusted_color( w_color::light_grey, being_hovered, being_clicked ) )
			->push_depth_nudge()
			->draw( subtex, subtex_rc );
	}

	// label

	if( control.label.length() )
	{
		w_rect label_rc = w_rect( label_pos.x, label_pos.y ) + base_offset;

#if 0
		if( subtex )
		{
			if( control.subtexture_align & align::left )
			{
				label_rc.x += subtex->get_bounding_rect().w / 2.0f;
			}
		}
#endif

		RENDER
			->push_depth_nudge()
			->push_rgb( _get_adjusted_color( w_color::light_grey, being_hovered, being_clicked ) )
			->push_align( align::centered );

		RENDER->draw_string( engine->pixel_font, control.label, label_rc );
	}

	// finish

	RENDER
		->end();
}

// takes a base color and modifies it based on the state of the UI

w_color w_imgui::_get_adjusted_color( w_color base_color, bool being_hovered, bool being_clicked )
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

	flow_right = { last_control->rc.x + last_control->rc.w + UI_PADDING, last_control->rc.y };
	flow_down = { last_control->rc.x, last_control->rc.y + last_control->rc.h + UI_PADDING };
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
