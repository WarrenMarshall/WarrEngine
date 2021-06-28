
#include "master_pch.h"
#include "master_header.h"

namespace war
{

unsigned ui_mgr::automatic_id;

ui_mgr::ui_mgr()
{
	caret_blink_tween = tween( 0.f, 1.f, 1000, tween_type::loop, tween_via::sinusoidal );
}

void ui_mgr::reset()
{
	mouse_cursor = g_engine->find_asset_safe<cursor_asset>( "mouse_cursor_default" );
	current_callback = &default_callback;
	automatic_id = 1;
}

ui_mgr* ui_mgr::panel_control( hash tag )
{
	return do_control<ui_panel_control>( tag );
}

ui_mgr* ui_mgr::caption_control( hash tag )
{
	return do_control<ui_caption_control>( tag );
}

ui_mgr* ui_mgr::button_control( hash tag )
{
	return do_control<ui_button_control>( tag );
}

ui_mgr* ui_mgr::check_control( hash tag )
{
	return do_control<ui_check_control>( tag );
}

ui_mgr* ui_mgr::divider_control( hash tag )
{
	return do_control<ui_divider_control>( tag );
}

ui_mgr* ui_mgr::spacer_control( hash tag )
{
	return do_control<ui_spacer_control>( tag );
}

ui_mgr* ui_mgr::image_control( hash tag )
{
	return do_control<ui_image_control>( tag );
}

ui_mgr* ui_mgr::label_control( hash tag )
{
	return do_control<ui_label_control>( tag );
}

ui_mgr* ui_mgr::slider_control( hash tag )
{
	return do_control<ui_slider_control>( tag );
}

ui_mgr* ui_mgr::text_control( hash tag )
{
	return do_control<ui_text_control>( tag );
}

ui_mgr* ui_mgr::radio_control( hash tag )
{
	return do_control<ui_radio_control>( tag );
}

ui_mgr* ui_mgr::progress_control( hash tag )
{
	return do_control<ui_progress_control>( tag );
}

ui_mgr* ui_mgr::list_control( hash tag )
{
	return do_control<ui_list_control>( tag );
}

ui_mgr* ui_mgr::dropdown_control( hash tag )
{
	return do_control<ui_dropdown_control>( tag );
}

ui_mgr* ui_mgr::set_text( std::string text )
{
	current_control->text = text;
	return this;
}

ui_mgr* ui_mgr::center_control_on_screen()
{
	// only valid for panel controls
	assert( current_control->type == ui_control_type::panel );

	vec2 centered_pos;

	centered_pos.x = ( ui_w - current_control->rc_ui.w ) / 2.f;
	centered_pos.y = ( ui_h - current_control->rc_ui.h ) / 2.f;

	set_pos( centered_pos );

	return this;
}

// looks at the current panel
ui_mgr* ui_mgr::adjust_layout_to_client_area()
{
	// only valid for panel controls
	assert( current_control->type == ui_control_type::panel );

	current_control->rc_client = compute_client_rect_from_ui_rect( current_control->rc_ui );

	// #layout : NOTE : this doesn't allow for panels within panels since this clears the
	// stack entirely but that can be added in the future if needed
	layout_init( current_control->rc_client );

	return this;
}

ui_mgr* ui_mgr::set_text_align( e_align align )
{
	current_control->text_align = align;
	return this;
}

ui_mgr* ui_mgr::set_image( std::string_view tex_tag )
{
	return set_image( g_engine->find_asset<texture_asset>( tex_tag ) );
}

ui_mgr* ui_mgr::set_image( texture_asset* tex )
{
	current_control->image = tex;

	return this;
}

ui_mgr* ui_mgr::set_func_draw( f_draw_control func_draw_control )
{
	current_control->func_draw_control = func_draw_control;
	return this;
}

ui_mgr* ui_mgr::set_slice_def( std::string_view slice_def_name )
{
	current_control->slice_def = nullptr;

	if( !slice_def_name.empty() )
	{
		current_control->slice_def = g_engine->find_asset<slice_def_asset>( slice_def_name );
	}

	current_control->rc_client = compute_client_rect_from_ui_rect( current_control->rc_ui );

	return this;
}

ui_mgr* ui_mgr::set_rect( const rect& rect )
{
	set_pos( { rect.x, rect.y } );
	set_size( { rect.w, rect.h } );

	return this;
}

ui_mgr* ui_mgr::set_pos( const vec2& pos )
{
	current_control->rc_ui.x = pos.x;
	current_control->rc_ui.y = pos.y;

	current_control->rc_client = compute_client_rect_from_ui_rect( current_control->rc_ui );
	current_control->set_pos_called = true;

	return this;
}

ui_mgr* ui_mgr::set_size( const vec2& sz )
{
	if( !fequals( sz.w, vec2::ignored ) )
	{
		current_control->rc_ui.w =
			fequals( sz.w, vec2::defaulted )
			? current_control->default_width
			: sz.w;
	}

	if( !fequals( sz.h, vec2::ignored ) )
	{
		current_control->rc_ui.h =
			fequals( sz.h, vec2::defaulted )
			? current_control->default_height
			: sz.h;
	}

	current_control->rc_client = compute_client_rect_from_ui_rect( current_control->rc_ui );

	return this;
}

ui_mgr* ui_mgr::set_interval( float interval )
{
	current_control->interval = interval;
	return this;
}

ui_mgr* ui_mgr::set_idx( int idx )
{
	current_control->idx = idx;
	return this;
}

// copies the window rect to the client rect, and then makes adjustments
// to compensate for graphical frames and whatever else would affect the client area.

rect ui_mgr::compute_client_rect_from_ui_rect( rect rc_ui )
{
	auto result = rc_ui;

	if( current_control->slice_def )
	{
		result.x += current_control->slice_def->patches[ slicedef_patch::top_left ]->rc.w;
		result.y += current_control->slice_def->patches[ slicedef_patch::top_left ]->rc.h;

		result.w -= current_control->slice_def->patches[ slicedef_patch::top_left ]->rc.w + current_control->slice_def->patches[ slicedef_patch::bottom_right ]->rc.w;
		result.h -= current_control->slice_def->patches[ slicedef_patch::top_left ]->rc.h + current_control->slice_def->patches[ slicedef_patch::bottom_right ]->rc.h;
	}

	return result;
}

bool ui_mgr::done()
{
	result = {};

	// not specifically telling the UI system what to do with your layout means
	// you are cut from the top of the current layout rect.
	if( !current_control->set_pos_called )
	{
		cut_top();
	}

	current_control->rc_client = compute_client_rect_from_ui_rect( current_control->rc_ui );

	if( current_control->is_active )
	{
		end_active_control();
	}
	else
	{
		end_static_control();
	}

	if( current_control->type == ui_control_type::panel )
	{
		// we always want panels to be below anything they contain, so nudge the
		// depth value a little before leaving
		render::state->z += zdepth_nudge;
	}

	return ( result.code & im_result::left_clicked );
}

void ui_mgr::end_static_control()
{
	result.code = im_result::none;
	result.click_pos = vec2::zero;
	result.click_pct = vec2::zero;

	draw( current_control.get(), false, false );
}

void ui_mgr::end_active_control()
{
	bool is_hot = ( hot_tag == current_control->tag );
	bool is_hovered = ( hover_tag == current_control->tag );

	// only the topmost scene is allowed to respond to UI input

	auto current_scene = g_engine->scenes.current_scene;

	if( current_scene == g_engine->scenes.get_top() )
	{
		range expanded_tag_range( current_scene->ui_expanded_tag_begin, current_scene->ui_expanded_tag_end );
		if( current_scene->ui_expanded_tag_begin == hash_none or expanded_tag_range.is_within( current_control->tag ) )
		{
			update_im_state( current_control.get(), is_hovered, is_hot );

			draw( current_control.get(), is_hovered, is_hot );

			if( is_hot and current_control->sticky_hover )
			{
				current_callback->on_motion( current_control->tag, result );
			}

			return;
		}
	}

	// by default, controls are drawn without any hot or hovering highlights.
	// this handles controls on inactive scenes or controls on a scene where
	// another control is expanded.

	draw( current_control.get(), false, false );
}

void ui_mgr::update_im_state( ui_control* control, bool is_hovered, bool is_hot )
{
	control->rc_ui.w =
		fequals( control->rc_ui.w, 0.f )
		? control->default_width
		: control->rc_ui.w;

	control->rc_ui.h =
		fequals( control->rc_ui.h, 0.f )
		? control->default_height
		: control->rc_ui.h;

	result.code = im_result::none;

	e_button_state bs_left = g_engine->input.get_button_state( input_id::mouse_button_left );

	auto rc_ui_offset = control->rc_ui;
	if( is_mouse_inside( rc_ui_offset ) )
	{
		if( bs_left == button_state::up or ( bs_left == button_state::held and g_ui->hot_tag == control->tag ) )
		{
			g_ui->hover_tag = control->tag;
		}
		else if( bs_left == button_state::pressed )
		{
			g_ui->hot_tag = control->tag;
		}
		else if( bs_left == button_state::released )
		{
			if( g_ui->hot_tag == control->tag and g_ui->hover_tag == control->tag )
			{
				result.code |= im_result::left_clicked;
				current_callback->on_control_left_clicked( control->tag, result );
			}
			g_ui->hover_tag = g_ui->hot_tag = hash_none;
		}
	}
	else
	{
		if( g_ui->hover_tag == control->tag and control->sticky_hover == false )
		{
			if( g_ui->hover_tag == control->tag )
			{
				g_ui->hover_tag = hash_none;
			}
		}

		if( bs_left == button_state::released and g_ui->hot_tag == control->tag )
		{
			g_ui->hot_tag = hash_none;
		}
	}

	if( g_ui->hover_tag == control->tag )
	{
		result.code |= im_result::hovered;
	}
	if( g_ui->hot_tag == control->tag )
	{
		result.code |= im_result::hot;
	}

	// client rect position of mouse cursor

	if( result.code & im_result::left_clicked or control->sticky_hover )
	{
		// convert mouse location to client rect position inside control

		auto viewport_pos = coord_system::window_to_viewport_pos( g_engine->input.mouse_window_pos );
		auto ui_pos = coord_system::viewport_to_ui_pos( viewport_pos );

		result.click_pos =
			ui_pos
			- vec2( current_control->rc_ui.x, current_control->rc_ui.y );

		result.click_pct.x = result.click_pos.x / current_control->rc_ui.w;
		result.click_pos.y = result.click_pos.y / current_control->rc_ui.h;

		result.click_pct.x = glm::clamp( result.click_pct.x, 0.f, 1.f );
		result.click_pct.y = glm::clamp( result.click_pct.y, 0.f, 1.f );
	}
}

void ui_mgr::draw( ui_control* control, bool is_hovered, bool is_hot )
{
	auto control_data = current_callback->get_data( control->tag );

	if( control->can_retain_focus and focused.tag == control->tag )
	{
		is_hot = true;
	}
	else
	{
		if( is_hot )
		{
			focused.tag = hash_none;
		}
	}

	// the rectangles that were used to reserve space for the control in the
	// layout were created with inner margins applied. those margins need to be
	// removed before we try to draw the control or it will be sized
	// incorrectly.

	auto margins = control->get_control_inner_margins();

	rect rc_ui = control->rc_ui;
	rc_ui.x += margins.x / 2.f;
	rc_ui.y += margins.y / 2.f;
	rc_ui.w -= margins.x;
	rc_ui.h -= margins.y;

	rect rc_client = control->rc_client;
	rc_client.x += margins.x / 2.f;
	rc_client.y += margins.y / 2.f;
	rc_client.w -= margins.x;
	rc_client.h -= margins.y;

	vec2 clicked_offset = get_click_offset( is_hovered, is_hot );
	rect rc_ui_offset = rc_ui + clicked_offset;
	rect rc_client_offset = rc_client + clicked_offset;

	{
		scoped_render_state;

		// if the expanded tag is being cleared, then force all control data to
		// be unexpanded before drawing this frame.
		if( g_engine->scenes.current_scene->flags.clear_expanded_tag_this_frame )
		{
			control_data->set_expanded( false );
		}

		if( control->func_draw_control )
		{
			control->func_draw_control( control, rc_ui_offset );
		}
		else
		{
			control->draw( rc_ui_offset, rc_client_offset, is_hovered, is_hot );
		}
	}

	if( is_hot and control->can_retain_focus )
	{
		focused.tag = control->tag;
		focused.type = control->type;
	}
}

// takes a base color and modifies it based on the state of the UI

color ui_mgr::get_adjusted_color( const color& base_color, bool is_hovered, bool is_hot )
{
	color color = base_color;

	if( is_hot )
	{
		color::scale( color, 1.75f );
	}
	else if( is_hovered )
	{
		color::scale( color, 1.25f );
	}

	return color;
}

ui_mgr* ui_mgr::layout_init()
{
	layout_stack.clear();
	layout_push( { 0.f, 0.f, ui_w, ui_h } );
	return this;
}

ui_mgr* ui_mgr::layout_init( rect rc )
{
	layout_stack.clear();
	layout_push( rc );

	return this;
}

ui_mgr* ui_mgr::layout_push()
{
	rect layout = { 0.f, 0.f, ui_w, ui_h };
	layout_stack.emplace_back( layout );

	return this;
}

ui_mgr* ui_mgr::layout_push( rect rc )
{
	layout_stack.emplace_back( rc );
	return this;
}

ui_mgr* ui_mgr::layout_pop()
{
	assert( !layout_stack.empty() );

	layout_stack.erase( layout_stack.begin() + ( layout_stack.size() - 1 ) );
	return this;
}

rect ui_mgr::layout_top()
{
	assert( !layout_stack.empty() );

	return layout_stack.back();
}

rect* ui_mgr::layout_top_ptr()
{
	assert( !layout_stack.empty() );

	return &(layout_stack.back());
}

ui_mgr* ui_mgr::cut_top( float sz )
{
	set_rect( layout_top_ptr()->cut_top( sz > 0.f ? sz : current_control->default_height + current_control->get_control_inner_margins().y ) );
	return this;

}

ui_mgr* ui_mgr::cut_bottom( float sz )
{
	set_rect( layout_top_ptr()->cut_bottom( sz > 0.f ? sz : current_control->default_height + current_control->get_control_inner_margins().y ) );
	return this;

}

ui_mgr* ui_mgr::cut_left( float sz )
{
	set_rect( layout_top_ptr()->cut_left( sz > 0.f ? sz : current_control->default_width + current_control->get_control_inner_margins().x ) );
	return this;

}

ui_mgr* ui_mgr::cut_right( float sz )
{
	set_rect( layout_top_ptr()->cut_right( sz > 0.f ? sz : current_control->default_width + current_control->get_control_inner_margins().x ) );
	return this;

}

void ui_mgr::draw_topmost()
{
	// mouse cursor

	if( mouse_cursor and g_engine->window.mouse_mode == mouse_mode::custom )
	{
		{
			scoped_render_state;

			render::state->z = zdepth_topmost;

			auto viewport_pos = coord_system::window_to_viewport_pos( g_engine->input.mouse_window_pos );
			auto ui_pos = coord_system::viewport_to_ui_pos( viewport_pos );

			render::draw_quad(
				mouse_cursor->texture,
				vec2( ( ui_pos.x - mouse_cursor->hotspot_offset.x ), ( ui_pos.y - mouse_cursor->hotspot_offset.y ) )
			);

		#if 0
			// helps with debugging the hotspot code
			//
			// draws a red dot on the actual mouse cursor position, as per Windows

			w_render::state->color = clr::red;
			w_render::draw_point( ui_pos );
		#endif
		}
	}
}

// a control with the mouse button held down on it will offset slightly

vec2 ui_mgr::get_click_offset( bool is_hovered, bool is_hot )
{
	if( current_control->uses_click_offset and is_hovered and is_hot )
	{
		return vec2( 1, 1 );
	}

	return vec2( 0, 0 );
}

// checks if the mouse cursor is inside the bounds of a rectangle.
// note : both the mouse position and the rectangle need to be in UI space

bool ui_mgr::is_mouse_inside( rect& rc )
{
	auto viewport_pos = coord_system::window_to_viewport_pos( g_engine->input.mouse_window_pos );
	auto ui_pos = coord_system::viewport_to_ui_pos( viewport_pos );

	//return c2AABBtoPoint( rc.to_c2AABB(), ui_pos.to_c2v() );
	return rc.contains_point( ui_pos );
}

}
