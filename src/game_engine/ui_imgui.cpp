
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

void w_imgui::clear_origin()
{
	origin_control = std::nullopt;
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
	control.slice_def = slice_def;
	return this;
}

w_imgui* w_imgui::set_subtexture( a_subtexture* subtexture )
{
	control.subtexture = subtexture;
	return this;
}

w_imgui* w_imgui::set_rect( w_rect rc )
{
	control.rc = rc;


	return this;
}

// takes the current control settings and saves them into the origin
// control so any controls afterwards will use them as a reference
// for positioning.

w_imgui* w_imgui::set_last_as_origin()
{
	origin_control = control;

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

w_imgui_result* w_imgui::active()
{
	im_automatic_id++;

	control.rc.x += origin_control ? origin_control->rc.x : 0.0f;
	control.rc.y += origin_control ? origin_control->rc.x : 0.0f;

	if( containing_layer_is_topmost )
	{
		control.result = update_im_state( im_automatic_id, control.rc );
	}

	draw( control, hover_id == im_automatic_id, hot_id == im_automatic_id );

	return &control.result;
}

w_imgui_result* w_imgui::passive()
{
	control.rc.x += origin_control ? origin_control->rc.x : 0.0f;
	control.rc.y += origin_control ? origin_control->rc.y : 0.0f;

	draw( control, false, false );

	return &control.result;
}

e_im_result w_imgui::update_im_state( int id, w_rect rc )
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

void w_imgui::draw( const w_imgui_control& control, bool being_hovered, bool being_clicked )
{
	w_offset clicked_offset = get_click_offset( being_hovered, being_clicked );

	w_rect rc_draw = control.rc;
	rc_draw.x += clicked_offset.x;
	rc_draw.y += clicked_offset.y;

	w_pos label_pos = rc_draw.midpoint();

	RENDER
		->begin();

	if( control.slice_def )
	{
		RENDER
			->push_depth_nudge()
			->push_rgb( get_adjusted_color( w_color::dark_grey, being_hovered, being_clicked ) )
			->draw_sliced( control.slice_def, rc_draw );
	}

	if( control.subtexture )
	{
		w_rect rc_img = rc_draw;

		// if there is a slice_def defined for this control, the image will default
		// to drawing inside the body of the control. meaning - offset from the top_left
		// corner by the size of the top_left corner graphic.
		if( control.slice_def )
		{
			rc_img.x += control.slice_def->patches[ slicedef_patch::P_00 ]->rc_tex.w;
			rc_img.y += control.slice_def->patches[ slicedef_patch::P_00 ]->rc_tex.h;

			rc_img.w -= control.slice_def->patches[ slicedef_patch::P_00 ]->rc_tex.w + control.slice_def->patches[ slicedef_patch::P_22 ]->rc_tex.w;
			rc_img.h -= control.slice_def->patches[ slicedef_patch::P_00 ]->rc_tex.h + control.slice_def->patches[ slicedef_patch::P_22 ]->rc_tex.h;
		}

		RENDER
			->push_rgb( get_adjusted_color( w_color::white, being_hovered, being_clicked ) )
			->push_depth_nudge()
			->draw( control.subtexture, rc_img );
	}

	if( control.label.length() )
	{
		RENDER
			->push_depth_nudge()
			->push_rgb( get_adjusted_color( w_color::light_grey, being_hovered, being_clicked ) )
			->push_align( align::centered );

		RENDER->draw_string( engine->pixel_font, control.label, w_rect( label_pos.x, label_pos.y ) );
	}

	RENDER
		->end();
}

// takes a base color and modifies it based on the state of the UI

w_color w_imgui::get_adjusted_color( w_color base_color, bool being_hovered, bool being_clicked )
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

// a control with the mouse button held down on it will offset slightly

w_offset w_imgui::get_click_offset( bool being_hovered, bool being_clicked )
{
	if( being_hovered && being_clicked )
	{
		return w_offset( 1, 1 );
	}

	return w_offset( 0, 0 );
}

