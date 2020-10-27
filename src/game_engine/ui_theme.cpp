
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

void w_ui_style::draw( std::string_view label, w_rect& rc, bool being_hovered, bool being_clicked )
{
	w_offset offset = get_click_offset( being_hovered, being_clicked );
	w_rect rc_draw = { rc.x + offset.x, rc.y + offset.y, rc.w, rc.h };

	w_pos label_pos = rc_draw.midpoint();

	RENDER
		->begin();

	if( slice_def )
	{
		RENDER
			->push_depth_nudge()
			->push_rgb( get_adjusted_color( w_color::dark_grey, being_hovered, being_clicked ) )
			->draw_sliced( slice_def, rc_draw );
	}

	if( subtexture )
	{
		w_rect rc_img = rc_draw;

		// if there is a slice_def defined for this control, the image will default
		// to drawing inside the body of the control. meaning - offset from the top_left
		// corner by the size of the top_left corner graphic.
		if( slice_def )
		{
			rc_img.x += slice_def->patches[ slicedef_patch::P_00 ]->rc_tex.w;
			rc_img.y += slice_def->patches[ slicedef_patch::P_00 ]->rc_tex.h;

			rc_img.w -= slice_def->patches[ slicedef_patch::P_00 ]->rc_tex.w + slice_def->patches[ slicedef_patch::P_22 ]->rc_tex.w;
			rc_img.h -= slice_def->patches[ slicedef_patch::P_00 ]->rc_tex.h + slice_def->patches[ slicedef_patch::P_22 ]->rc_tex.h;
		}

		RENDER->push_rgb( get_adjusted_color( w_color::white, being_hovered, being_clicked ) )
			->push_depth_nudge()
			->draw( subtexture, rc_img );
	}

	if( label.length() )
	{
		RENDER
			->push_depth_nudge()
			->push_rgb( get_adjusted_color( w_color::light_grey, being_hovered, being_clicked ) )
			->push_align( align::centered );

		RENDER->draw_string( engine->pixel_font, label,
								w_rect(
									label_pos.x,
									label_pos.y )
		);
	}

	RENDER
		->end();
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
		return w_offset( 1, 1 );
	}

	return w_offset( 0, 0 );
}

// ----------------------------------------------------------------------------

#if 0
void w_ui_theme::init()
{
	mouse_cursor = nullptr;// engine->get_asset<a_cursor>( "ui_cursor", b_silent( true ) );

	sd_panel = a_9slice_def::find( "sd_ui_panel" );
	sd_push_button = a_9slice_def::find( "sd_push_button" );
	st_checkbox_clear = a_subtexture::find( "ui_box" );
	st_checkbox_checked = a_subtexture::find( "ui_box_checkmark" );

	//default_button_style = std::make_unique<w_ui_style_button>();
	//default_button_style->base_attrib.color = w_color::dark_grey;
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
#endif
