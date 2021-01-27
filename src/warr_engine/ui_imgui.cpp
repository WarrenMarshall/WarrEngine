
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

void w_imgui_result::operator=( const e_im_result res )
{
	code = res;
}

bool w_imgui_result::was_left_clicked()
{
	return ( code == im_result::left_clicked );
}

// ----------------------------------------------------------------------------

void w_imgui::reset()
{
	im_automatic_id = 0;
	containing_layer_is_topmost = false;
	last_control = std::nullopt;
}

void w_imgui::set_current_callback_from_current_layer()
{
	current_callback = LAYER->get_imgui_callback();
}

w_imgui* w_imgui::do_panel( hash tag )
{
	set_current_callback_from_current_layer();

	current_control = {};
	current_control.type = imgui_control_type::panel;
	current_control.tag = tag;
	current_control.is_active = false;
	current_control.slice_def = a_9slice_def::find( "simple_ui_panel" );
	current_control.text_align = align::hcenter | align::vcenter;

	set_size( { w_sz::def, w_sz::def } );

	return this;
}

w_imgui* w_imgui::do_push_button( hash tag )
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

w_imgui* w_imgui::do_checkbox( hash tag )
{
	set_current_callback_from_current_layer();

	current_control = {};
	current_control.type = imgui_control_type::check_box;
	current_control.tag = tag;
	current_control.is_active = true;
	current_control.text_align = align::left | align::vcenter;
	current_control.uses_click_offset = false;

	set_size( { w_sz::def, w_sz::def } );

	return this;
}

w_imgui* w_imgui::do_divider( hash tag )
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

w_imgui* w_imgui::do_label( hash tag )
{
	set_current_callback_from_current_layer();

	current_control = {};
	current_control.type = imgui_control_type::label;
	current_control.tag = tag;
	current_control.is_active = false;
	current_control.text_align = align::left | align::vcenter;

	set_size( { w_sz::def, w_sz::def } );

	return this;
}

w_imgui* w_imgui::do_slider( hash tag )
{
	set_current_callback_from_current_layer();

	current_control = {};
	current_control.type = imgui_control_type::slider;
	current_control.tag = tag;
	current_control.is_active = true;
	current_control.text_align = align::left | align::vcenter;
	current_control.slice_def = a_9slice_def::find( "simple_ui_slider_body" );
	current_control.sticky_hover = true;
	current_control.uses_click_offset = false;

	set_size( { w_sz::def, w_sz::def } );

	return this;
}

w_imgui* w_imgui::do_edit_box( hash tag )
{
	set_current_callback_from_current_layer();

	current_control = {};
	current_control.type = imgui_control_type::edit_box;
	current_control.tag = tag;
	current_control.is_active = true;
	current_control.slice_def = a_9slice_def::find( "simple_ui_edit_box" );
	current_control.text_align = align::left | align::vcenter;
	current_control.uses_click_offset = false;
	current_control.sticky_hot = true;

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
	current_control.set_position_called = true;

	return this;
}

w_imgui* w_imgui::set_position( e_imgui_flow flow )
{
	current_control.rc_win.x = last_control->rc_win.x;
	current_control.rc_win.y = last_control->rc_win.y;

	if( flow & imgui_flow::right )
	{
		current_control.rc_win.x = flow_right_pos.x;
		current_control.rc_win.y = flow_right_pos.y;
	}

	if( flow & imgui_flow::down )
	{
		current_control.rc_win.x = flow_down_pos.x;
		current_control.rc_win.y = flow_down_pos.y;
	}

	if( flow & imgui_flow::last_crc_topleft )
	{
		current_control.rc_win.x = last_control->rc_client.x;
		current_control.rc_win.y = last_control->rc_client.y;
	}

	compute_clientrect_from_rect();

	current_control.set_position_called = true;
	return this;
}

w_imgui* w_imgui::set_size( const w_sz& sz )
{
	if( sz.w != w_sz::ignore )
	{
		current_control.rc_win.w =
			fequals( sz.w, w_sz::def )
			? current_callback->get_default_width( current_control )
			: sz.w;
	}
	if( sz.h != w_sz::ignore )
	{
		current_control.rc_win.h =
			fequals( sz.h, w_sz::def )
			? current_callback->get_default_height( current_control )
			: sz.h;
	}

	compute_clientrect_from_rect();

	return this;
}

w_imgui* w_imgui::set_interval( const float interval )
{
	current_control.interval = interval;
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
	// if this control was never given a position directly,
	// assume it wants to flow down from the last control.
	if( !current_control.set_position_called )
	{
		set_position( imgui_flow::down );
	}

	if( current_control.is_active )
	{
		im_automatic_id++;

		bool is_hot = ( hot_id == im_automatic_id );
		bool is_hovered = ( hover_id == im_automatic_id );

		if( containing_layer_is_topmost )
		{
			update_im_state( im_automatic_id, current_control, is_hovered, is_hot );
		}

		draw( current_control, is_hovered, is_hot );

		if( result.was_left_clicked() )
		{
			current_callback->on_left_clicked( current_control, result );
		}

		if( is_hot && current_control.sticky_hover )
		{
			current_callback->on_motion( current_control, result );
		}
	}
	else
	{
		result.code = im_result::none;
		result.click_pos = w_vec2::zero;
		result.click_pct = w_vec2::zero;

		draw( current_control, false, false );
	}

	set_as_last_control( current_control );

	return &result;
}

void w_imgui::update_im_state( int id, const w_imgui_control& control, bool is_hovered, bool is_hot )
{
	assert( control.rc_win.w );
	assert( control.rc_win.h );

	result.code = im_result::none;

	e_button_state bs_left = engine->input->get_button_state( input_id::mouse_button_left );
	bool mouse_is_inside = UI->is_mouse_inside( control.rc_win );

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
				result.code |= im_result::left_clicked;
			}
			IMGUI->hover_id = IMGUI->hot_id = -1;
		}
	}
	else
	{
		if( IMGUI->hover_id == id && control.sticky_hover == false )
		{
			if( IMGUI->hover_id == id )
			{
				IMGUI->hover_id = -1;
			}
		}

		if( bs_left == button_state::released && IMGUI->hot_id == id )
		{
			IMGUI->hot_id = -1;
		}
	}

	if( IMGUI->hover_id == id )
	{
		result.code |= im_result::hovered;
	}
	if( IMGUI->hot_id == id )
	{
		result.code |= im_result::hot;
	}

	// client rect position of mouse cursor

	if( result.code == im_result::left_clicked || control.sticky_hover )
	{
		// convert mouse location to client rect position inside control
		result.click_pos.x = engine->input->mouse_uiwindow_pos.x - current_control.rc_win.x;
		result.click_pos.y = engine->input->mouse_uiwindow_pos.y - current_control.rc_win.y;

		result.click_pct.x = result.click_pos.x / current_control.rc_win.w;
		result.click_pos.y = result.click_pos.y / current_control.rc_win.h;

		result.click_pct.x = glm::clamp( result.click_pct.x, 0.0f, 1.0f );
		result.click_pct.y = glm::clamp( result.click_pct.y, 0.0f, 1.0f );
	}
}

void w_imgui::draw( w_imgui_control& control, bool is_hovered, bool is_hot )
{
	if( control.sticky_hot )
	{
		if( current_callback->tag_sticky_hot == control.tag )
		{
			is_hot = true;
		}
	}
	else
	{
		if( is_hot )
		{
			current_callback->tag_sticky_hot = hash_none;
		}
	}

	w_vec2 clicked_offset = get_click_offset( is_hovered, is_hot );

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
			draw_slice_def( control, rc_win_offset, is_hovered, is_hot );

			// text on a panel translates to a caption bar at the
			// top of the panel.
			//
			// the client area of the panel is then adjusted to remove
			// the space used for the caption bar.

			if( !control.text.empty() && control.slice_def )	// can't have a caption without a slicedef
			{
				// caption area starts out as matching the client rect,
				// then gets shrunk to match the font height.
				w_rect rc = control.rc_client;
				rc.h = engine->pixel_font->font_def->max_height;

				// background bar that sits behind the caption text
				w_rect rc_label_background( control.rc_client );
				rc_label_background.h = engine->pixel_font->font_def->max_height + current_callback->get_control_margin();
				RENDER
					->push_rgb( w_color::pal( 0 ) )
					->draw_filled_rectangle( rc_label_background );

				// caption text
				draw_text( control, rc, w_color::pal( 3 ), is_hovered, is_hot );

				// panel client rect is adjusted so that it sits below the caption area
				control.rc_client.y += rc.h + current_callback->get_control_padding();
				control.rc_client.h -= rc.h;

				control.rc_client = control.rc_client;
			}
		}
		break;

		case imgui_control_type::push_button:
		{
			draw_slice_def( control, rc_win_offset, is_hovered, is_hot );
			draw_text( control, rc_client_offset, w_color::pal( 2 ), is_hovered, is_hot );
		}
		break;

		case imgui_control_type::check_box:
		{
			a_texture* texture = current_callback->get_texture_for_checkbox( control );

			draw_slice_def( control, rc_win_offset, is_hovered, is_hot );

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

			draw_texture( control, rc_texture, texture, is_hovered, is_hot );
			draw_text( control, rc_client_offset, w_color::pal( 2 ), is_hovered, is_hot );
		}
		break;

		case imgui_control_type::label:
		{
			draw_slice_def( control, rc_win_offset, false, false );
			draw_text( control, rc_client_offset, w_color::pal( 2 ), false, false );
		}
		break;

		case imgui_control_type::divider:
		{
			draw_slice_def( control, rc_win_offset, false, false );
		}
		break;

		case imgui_control_type::slider:
		{
			draw_slice_def( control, rc_win_offset, is_hovered, is_hot );

			w_vec2 pos = {
				rc_client_offset.x + ( rc_client_offset.w * current_callback->get_data_for_control( control ) ),
				rc_client_offset.y + ( rc_client_offset.h / 2.0f )
			};

			RENDER->push_rgb( get_adjusted_color( w_color::pal( 2 ), is_hovered, is_hot ) );

			// draw tick marks if this slider is using an interval

			if( control.interval )
			{
				auto tex_tick = a_texture::find( "ui_slider_tick" );

				w_pos tick_pos = { rc_client_offset.x, control.rc_win.y + 4 };
				auto steps = (int)( 1.0f / control.interval );
				auto stride = rc_client_offset.w * control.interval;

				for( auto x = 0 ; x <= steps ; ++x )
				{
					RENDER->draw_sprite( tex_tick, tick_pos );
					tick_pos.x += stride;
				}
			}

			// draw the thumb indicator
			RENDER->draw_sprite( a_texture::find( "ui_slider_thumb" ), pos );
		}
		break;

		case imgui_control_type::edit_box:
		{
			draw_slice_def( control, rc_win_offset, is_hovered, is_hot );
		}
		break;
	}

	RENDER->end();

	if( is_hot && control.sticky_hot )
	{
		current_callback->tag_sticky_hot = control.tag;
	}
}

void w_imgui::draw_slice_def( const w_imgui_control& control, const w_rect& rc_win, bool is_hovered, bool is_hot )
{
	if( control.slice_def )
	{
		RENDER->push_depth_nudge()
			->push_rgb( get_adjusted_color( w_color::pal( 1 ), is_hovered, is_hot ) )
			->draw_sliced( control.slice_def, rc_win );
	}
}

void w_imgui::draw_texture( const w_imgui_control& control, const w_rect& rc, const a_texture* texture, bool is_hovered, bool is_hot )
{
	RENDER->push_depth_nudge()
		->push_rgb( get_adjusted_color( w_color::pal( 2 ), is_hovered, is_hot ) )
		->draw_sprite( texture, rc.midpoint() );
}

void w_imgui::draw_text( const w_imgui_control& control, const w_rect& rc_client, const w_color& color, bool is_hovered, bool is_hot )
{
	if( control.text.length() )
	{
		const w_pos pos = rc_client.get_position_from_alignment( control.text_align );

		w_render_state_opt rso;
		rso.color = get_adjusted_color( color, is_hovered, is_hot );
		rso.align = control.text_align;

		RENDER
			->push_render_state( rso )
			->draw_string( control.text, pos )
			->pop();
	}
}

// takes a base color and modifies it based on the state of the UI

w_color w_imgui::get_adjusted_color( const w_color& base_color, bool is_hovered, bool is_hot )
{
	w_color color = base_color;

	if( is_hot )
	{
		w_color::scale( color, 1.75f );
	}
	else if( is_hovered )
	{
		w_color::scale( color, 1.25f );
	}

	return color;
}

void w_imgui::set_as_last_control( w_imgui_control control )
{
	last_control = control;

	flow_right_pos = { last_control->rc_win.x + last_control->rc_win.w + current_callback->get_control_margin(), last_control->rc_win.y };
	flow_down_pos = { last_control->rc_win.x, last_control->rc_win.y + last_control->rc_win.h + current_callback->get_control_margin() };
}

// a control with the mouse button held down on it will offset slightly

w_offset w_imgui::get_click_offset( bool is_hovered, bool is_hot )
{
	if( current_control.uses_click_offset && is_hovered && is_hot )
	{
		return w_offset( 1, 1 );
	}

	return w_offset( 0, 0 );
}
