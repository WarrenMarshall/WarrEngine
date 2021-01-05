
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

w_imgui* w_imgui::init_panel( const std::string& tag )
{
	current_control = {};
	current_control.type = imgui_control_type::panel;
	current_control.tag = tag;
	current_control.is_active = false;
	current_control.slice_def = a_9slice_def::find( "simple_ui_panel" );
	current_control.label_align = align::left | align::top;

	return this;
}

w_imgui* w_imgui::init_push_button( const std::string& tag )
{
	current_control = {};
	current_control.type = imgui_control_type::push_button;
	current_control.tag = tag;
	current_control.is_active = true;
	current_control.slice_def = a_9slice_def::find( "simple_ui_push_button" );
	current_control.label_align = align::centered;

	return this;
}

w_imgui* w_imgui::init_checkbox( const std::string& tag )
{
	current_control = {};
	current_control.type = imgui_control_type::check_box;
	current_control.tag = tag;
	current_control.is_active = true;
	current_control.label_align = align::left | align::vcenter;

	return this;
}

w_imgui* w_imgui::set_label( const std::string& label )
{
	current_control.label = label;
	return this;
}

w_imgui* w_imgui::set_label_align( e_align align )
{
	current_control.label_align = align;
	return this;
}

w_imgui* w_imgui::set_slice_def( a_9slice_def* slice_def )
{
	current_control.slice_def = slice_def;
	return this;
}

w_imgui* w_imgui::set_texture( e_imgui_control_state state, a_texture* texture )
{
	current_control.textures[ state ] = texture;

	return this;
}

w_imgui* w_imgui::set_texture_align( e_align align )
{
	current_control.texture_align = align;
	return this;
}

w_imgui* w_imgui::set_position( const w_vec2& pos )
{
	current_control.rc = last_control->rc;

	current_control.rc.x = pos.x;
	current_control.rc.y = pos.y;

	compute_clientrect_from_rect();

	return this;
}

w_imgui* w_imgui::set_position( e_imgui_flow flow )
{
	current_control.rc = last_control->rc;

	if( flow & imgui_flow::right )
	{
		current_control.rc.x = flow_right.x;
		current_control.rc.y = flow_right.y;
	}

	if( flow & imgui_flow::down )
	{
		current_control.rc.x = flow_down.x;
		current_control.rc.y = flow_down.y;
	}

	if( flow & imgui_flow::last_crc_topleft )
	{
		current_control.rc.x = last_control->crc.x;
		current_control.rc.y = last_control->crc.y;
	}

	compute_clientrect_from_rect();

	return this;
}

w_imgui* w_imgui::set_size( const w_sz& size )
{
	current_control.rc.w = size.w;
	current_control.rc.h = size.h;

	compute_clientrect_from_rect();

	return this;
}

// copies the overall control rect to the client rect, and then makes adjustments
// to compensate for graphical frames and whatever else would affect the client area.

void w_imgui::compute_clientrect_from_rect()
{
	current_control.crc = current_control.rc;

	if( current_control.slice_def )
	{
		current_control.crc.x += current_control.slice_def->patches[ slicedef_patch::P_00 ]->rc.w;
		current_control.crc.y += current_control.slice_def->patches[ slicedef_patch::P_00 ]->rc.h;

		current_control.crc.w -= current_control.slice_def->patches[ slicedef_patch::P_00 ]->rc.w + current_control.slice_def->patches[ slicedef_patch::P_22 ]->rc.w;
		current_control.crc.h -= current_control.slice_def->patches[ slicedef_patch::P_00 ]->rc.h + current_control.slice_def->patches[ slicedef_patch::P_22 ]->rc.h;
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

	if( !current_control.tag.empty() )
	{
		tagged_controls.insert( std::pair( current_control.tag, current_control ) );
	}

	_set_as_last_control( current_control );

	w_imgui_callback* callback = nullptr;

	if( IMGUI->containing_layer_is_topmost )
	{
		callback = LAYER->get_imgui_callback();
	}

	if( callback )
	{
		if( result.was_left_clicked() )
		{
			callback->was_left_clicked( &current_control );
		}
		if( result.was_right_clicked() )
		{
			callback->was_right_clicked( &current_control );
		}
	}

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

e_im_result w_imgui::_update_im_state( int id, const w_rect& rc )
{
	assert( rc.w );
	assert( rc.h );

	e_im_result imresult = im_result::none;

	/*
		 reduce the size of the hit rectangle. this gives more breathing room
		 when mousing over tightly packed UI elements and stops the user
		 from being able to highlight/click more than one at a time.
	*/

	w_rect rc_hit = w_rect( rc.x, rc.y, rc.w - 1.0f, rc.h - 1.0f );

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

void w_imgui::_draw( w_imgui_control& control, bool being_hovered, bool being_clicked )
{
	w_imgui_callback* callback = nullptr;
	if( IMGUI->containing_layer_is_topmost )
	{
		callback = LAYER->get_imgui_callback();
	}

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
			->push_rgb( _get_adjusted_color( w_color::pal( 1 ), being_hovered, being_clicked ) )
			->draw_sliced( control.slice_def, rc_draw );
	}

	// texture

	// #ui - this is where checkbox needs to get it's texture drawn
	a_texture* texture = nullptr;

	if( callback )
	{
		auto idx = callback->get_state_for_control( &control ) == imgui_control_state::checked ? 1 : 0;
		texture = control.textures[ idx ];
	}

	if( texture )
	{
		w_rect rc = control.crc + clicked_offset;

		if( control.texture_align & align::left )
		{
			rc.w = texture->rc.w;
			rc.h = texture->rc.h;
		}

		RENDER
			->push_rgb( _get_adjusted_color( w_color::pal( 2 ), being_hovered, being_clicked ) )
			->push_depth_nudge()
			->draw( texture, rc );
	}

	// label

	if( control.label.length() )
	{
		w_rect label_rc = w_rect( label_pos.x, label_pos.y, current_control.crc.w, current_control.crc.h );

		RENDER
			->push_depth_nudge()
			->push_rgb( _get_adjusted_color( w_color::pal( 2 ), being_hovered, being_clicked ) )
			->push_align( control.label_align );

		RENDER->draw_string( control.label, label_rc );
	}

	// finish

	RENDER
		->end();
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
