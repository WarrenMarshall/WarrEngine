
#include "master_pch.h"
#include "master_header.h"

e_im_result w_ui_style::get_im_state( int id, w_rect rc )
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
		if( bs_left == button_state::up || ( bs_left == button_state::held && UI->hot_id == id ) )
		{
			UI->hover_id = id;
		}
		else if( bs_left == button_state::pressed )
		{
			UI->hot_id = id;
		}
		else if( bs_left == button_state::released )
		{
			if( UI->hot_id == id && UI->hover_id == id )
			{
				imresult |= im_result::left_clicked;
			}
			UI->hover_id = UI->hot_id = -1;
		}
	}
	else
	{
		if( UI->hover_id == id )
		{
			UI->hover_id = -1;
		}

		if( bs_left == button_state::released && UI->hot_id == id )
		{
			UI->hot_id = -1;
		}
	}

	if( UI->hover_id == id )
	{
		imresult |= im_result::hovered;
	}
	if( UI->hot_id == id )
	{
		imresult |= im_result::hot;
	}

	return imresult;
}

w_color w_ui_style::get_adjusted_color( w_color base_color, bool being_hovered, bool being_clicked )
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

w_offset w_ui_style::get_click_offset( bool being_hovered, bool being_clicked )
{
	if( being_hovered && being_clicked )
	{
		return clicked_offset;
	}

	return w_offset( 0, 0 );
}

// ----------------------------------------------------------------------------

w_ui_style_button::w_ui_style_button()
{
	slice_def = UI->theme->button_down_slice_def;


	label_attrib.alignment = align::centered;
}

void w_ui_style_button::draw( std::string_view label, w_rect& rc, bool being_hovered, bool being_clicked )
{
	w_offset offset = get_click_offset( being_hovered, being_clicked );
	w_rect rc_draw = { rc.x + offset.x, rc.y + offset.y, rc.w, rc.h };

	w_pos label_pos = { rc_draw.x + ( rc_draw.w / 2 ), rc_draw.y + ( rc_draw.h / 2 ) };

	RENDER
		->begin()
		->push_depth_nudge();

	if( slice_def.has_value() )
	{
		RENDER->push_rgb( get_adjusted_color( base_attrib.color, being_hovered, being_clicked ) )
			->draw_sliced( slice_def.value(), rc_draw );
	}

	/*
	if( subtex.has_value() )
	{
		w_rect rc_client = rc_draw;

		// if there is a slice_def defined for this control, the image will default
		// to drawing inside the body of the control. meaning - offset from the top_left
		// corner by the size of the top_left corner graphic.
		rc_client.x += slice_def.has_value() ? slice_def.value()->patches[ slicedef_patch::P_00 ]->sz._width : 0;
		rc_client.y += slice_def.has_value() ? slice_def.value()->patches[ slicedef_patch::P_00 ]->sz._height : 0;

		// if there are additional position tweaks, apply them
		rc_client.x += subtex_attrib.pos.value_or( w_vec2::zero ).x;
		rc_client.y += subtex_attrib.pos.value_or( w_vec2::zero ).y;

		// if there are size tweaks, apply them
		rc_client.w = subtex.value()->sz._width;
		if( subtex_attrib.sz.has_value() )
		{
			rc_client.w = ( *subtex_attrib.sz )._width;
		}

		rc_client.h = subtex.value()->sz._height;
		if( subtex_attrib.sz.has_value() )
		{
			rc_client.h = ( *subtex_attrib.sz )._height;
		}

		RENDER->push_rgb( get_adjusted_color( subtex_attrib.color, being_hovered, being_clicked ) )
			->push_depth_nudge()
			->draw( subtex.value(), rc_client );
	}
	*/

	if( label.length() )
	{
		RENDER
			->push_rgb( get_adjusted_color( label_attrib.color, being_hovered, being_clicked ) )
			->push_align( label_attrib.alignment )
			->push_depth_nudge();

		RENDER->draw_string( engine->pixel_font, label,
						   w_rect(
							   label_pos.x + label_attrib.pos.value_or( w_vec2::zero ).x,
							   label_pos.y + label_attrib.pos.value_or( w_vec2::zero ).y )
			);
	}

	RENDER
		->end();
}

// ----------------------------------------------------------------------------

w_ui_style_panel::w_ui_style_panel()
{
	slice_def = UI->theme->panel_slice_def;
}

void w_ui_style_panel::draw( std::string_view label, w_rect& rc, bool being_hovered, bool being_clicked )
{
	RENDER
		->begin()
		->push_depth_nudge()
		->push_rgb( base_attrib.color )
		->draw_sliced( slice_def, rc )
		->end();
}

// ----------------------------------------------------------------------------

void w_ui_theme::init()
{
	mouse_cursor = nullptr;// engine->get_asset<a_cursor>( "ui_cursor", b_silent( true ) );

	panel_slice_def = engine->get_asset<a_9slice_def>( "ui_default_panel" );
	button_down_slice_def = engine->get_asset<a_9slice_def>( "ui_default_button_down" );

	default_button_style = std::make_unique<w_ui_style_button>();
	default_button_style->base_attrib.color = w_color::dark_grey;
}

// called at the end of each frame, this is for drawing anything that
// needs to appear on top of everything else. like the mouse cursor.

void w_ui_theme::draw_topmost()
{
	// mouse cursor

	if( mouse_cursor && engine->window->mouse_mode == mouse_mode::hidden )
	{
		RENDER
			->begin()
			->push_depth( zdepth_topmost )
			->draw( mouse_cursor->subtex,
					w_rect(
						engine->input->mouse_vwindow_pos.x - mouse_cursor->hotspot_offset.x,
						engine->input->mouse_vwindow_pos.y - mouse_cursor->hotspot_offset.y
					)
			)
			->end();
	}
}
