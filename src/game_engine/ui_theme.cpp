
#include "master_pch.h"
#include "master_header.h"

e_im_result w_ui_style::update_im_state( int id, w_rect rc )
{
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
				//static a_sound* snd_click = engine->get_asset<a_sound>( "ui_click_01" );
				//snd_click->play();
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
	slice_def = UI->theme->button_slice_def;
}

void w_ui_style_button::draw( std::string_view label, w_rect& rc, bool being_hovered, bool being_clicked )
{
	w_offset offset = get_click_offset( being_hovered, being_clicked );
	w_rect rc_draw = { rc.x + offset.x, rc.y + offset.y, rc.w, rc.h };

	w_pos label_pos = { rc_draw.x + ( rc_draw.w / 2 ), rc_draw.y + ( rc_draw.h / 2 ) };
	e_align label_align = align::centered;

	RENDER
		->begin()
		->push_depth_nudge();

	if( slice_def )
	{
		RENDER->push_rgb( get_adjusted_color( w_color::dark_grey, being_hovered, being_clicked ) )
			->draw_sliced( slice_def, rc_draw );
	}

	if( subtex )
	{
		w_rect rc_client = rc_draw;

		rc_client.x += slice_def ? slice_def->patches[ slicedef_patch::P_00 ]->sz.w : 0;
		rc_client.y += slice_def ? slice_def->patches[ slicedef_patch::P_00 ]->sz.h : 0;

		rc_client.x += subtex_pos_offset.x * ( subtex_pos_offset.x != -1 );
		rc_client.y += subtex_pos_offset.y * ( subtex_pos_offset.y != -1 );

		rc_client.w = subtex_sz.w == -1 ? subtex->rc_src.w : subtex_sz.w;
		rc_client.h = subtex_sz.h == -1 ? subtex->rc_src.h : subtex_sz.h;

		RENDER->push_rgb( get_adjusted_color( w_color::light_grey, being_hovered, being_clicked ) )
			->push_depth_nudge()
			->draw( subtex, rc_client );

		label_pos = { rc_draw.x + rc_client.w + UI->theme->control_padding, rc_draw.y + ( rc_client.h / 2 ) };
		label_align = align::left | align::vcenter;
	}

	if( label.length() )
	{
		RENDER
			->push_rgb( get_adjusted_color( w_color::light_grey, being_hovered, being_clicked ) )
			->push_align( label_align )
			->push_depth_nudge()
			->draw_string( engine->pixel_font, label, w_rect( label_pos.x, label_pos.y, -1, -1 ) );
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
		->push_rgb( color )
		->draw_sliced( slice_def, rc )
		->end();
}

// ----------------------------------------------------------------------------

void w_ui_theme::init()
{
	mouse_cursor = engine->get_asset<a_cursor>( "ui_cursor", b_silent( true ) );
	engine->pixel_font = engine->get_asset<a_font>( "engine_pixel_font" );
	large_font = engine->get_asset<a_font>( "larger_font" );

	panel_slice_def = engine->get_asset<a_9slice_def>( "ui_simple_panel" );
	button_slice_def = engine->get_asset<a_9slice_def>( "ui_simple_panel" );
}

void w_ui_theme::draw_topmost()
{
	// mouse cursor

	if( engine->ui->mouse_is_visible && mouse_cursor != nullptr )
	{
		RENDER
			->begin()
			->push_depth( zdepth_engine )
			->draw( mouse_cursor->subtex,
					w_rect(
						engine->input->mouse_vwindow_pos.x - mouse_cursor->hotspot_offset.x,
						engine->input->mouse_vwindow_pos.y - mouse_cursor->hotspot_offset.y
					)
			)
			->end();
	}
}
