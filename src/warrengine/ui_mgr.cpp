
#include "master_pch.h"
#include "master_header.h"

namespace war
{

uint32_t UI_Mgr::automatic_id;

UI_Mgr::UI_Mgr()
{
	caret_blink_tween = Tween( 0.f, 1.f, 1000, e_tween_type::loop, e_tween_via::sinusoidal );
}

void UI_Mgr::reset()
{
	mouse_cursor = g_engine->find_asset_safe<Cursor_Asset>( "mouse_cursor_default" );
	current_callback = &default_callback;
	automatic_id = 1;
}

UI_Mgr* UI_Mgr::panel_control( hash tag )
{
	return do_control( e_ui_control_type::panel, tag );
}

UI_Mgr* UI_Mgr::caption_control( hash tag )
{
	return do_control( e_ui_control_type::caption, tag );
}

UI_Mgr* UI_Mgr::button_control( hash tag )
{
	return do_control( e_ui_control_type::button, tag );
}

UI_Mgr* UI_Mgr::check_control( hash tag )
{
	return do_control( e_ui_control_type::check, tag );
}

UI_Mgr* UI_Mgr::divider_control( hash tag )
{
	return do_control( e_ui_control_type::divider, tag );
}

UI_Mgr* UI_Mgr::spacer_control( hash tag )
{
	return do_control( e_ui_control_type::spacer, tag );
}

UI_Mgr* UI_Mgr::image_control( hash tag )
{
	return do_control( e_ui_control_type::image, tag );
}

UI_Mgr* UI_Mgr::label_control( hash tag )
{
	return do_control( e_ui_control_type::label, tag );
}

UI_Mgr* UI_Mgr::slider_control( hash tag )
{
	return do_control( e_ui_control_type::slider, tag );
}

UI_Mgr* UI_Mgr::text_control( hash tag )
{
	return do_control( e_ui_control_type::text, tag );
}

UI_Mgr* UI_Mgr::radio_control( hash tag )
{
	return do_control( e_ui_control_type::radio, tag );
}

UI_Mgr* UI_Mgr::progress_control( hash tag )
{
	return do_control( e_ui_control_type::progress, tag );
}

UI_Mgr* UI_Mgr::list_control( hash tag )
{
	return do_control( e_ui_control_type::list, tag );
}

UI_Mgr* UI_Mgr::dropdown_control( hash tag )
{
	return do_control( e_ui_control_type::dropdown, tag );
}

UI_Mgr* UI_Mgr::do_control( e_ui_control_type control_type, hash tag )
{
	current_control = std::make_unique<UI_Control>();

	switch( control_type )
	{
		case e_ui_control_type::panel:
			current_control->slice_def = g_engine->find_asset<Slice_Def_Asset>( "simple_ui_panel" );
			break;

		case e_ui_control_type::caption:
			current_control->text_align = e_align::hcenter | e_align::vcenter;
			current_control->primary_color = make_color( e_pal::darkest );
			break;

		case e_ui_control_type::button:
			current_control->is_active = true;
			current_control->slice_def = g_engine->find_asset<Slice_Def_Asset>( "simple_ui_button" );
			current_control->text_align = e_align::centered;
			break;

		case e_ui_control_type::check:
			current_control->is_active = true;
			current_control->text_align = e_align::left | e_align::vcenter;
			break;

		case e_ui_control_type::divider:
			current_control->slice_def = g_engine->find_asset<Slice_Def_Asset>( "simple_ui_divider" );
			break;

		case e_ui_control_type::label:
			current_control->text_align = e_align::left | e_align::vcenter;
			break;

		case e_ui_control_type::slider:
			current_control->is_active = true;
			current_control->text_align = e_align::left | e_align::vcenter;
			current_control->slice_def = g_engine->find_asset<Slice_Def_Asset>( "simple_ui_slider_body" );
			current_control->uses_click_offset = false;
			break;

		case e_ui_control_type::text:
			current_control->is_active = true;
			current_control->slice_def = g_engine->find_asset<Slice_Def_Asset>( "simple_ui_text" );
			current_control->text_align = e_align::left | e_align::vcenter;
			current_control->uses_click_offset = false;
			current_control->can_retain_focus = true;
			break;

		case e_ui_control_type::radio:
			current_control->is_active = true;
			current_control->text_align = e_align::left | e_align::vcenter;
			break;

		case e_ui_control_type::list:
			current_control->text_align = e_align::left | e_align::top;
			current_control->slice_def = g_engine->find_asset<Slice_Def_Asset>( "simple_ui_list" );
			break;

		case e_ui_control_type::dropdown:
			current_control->is_active = true;
			current_control->slice_def = g_engine->find_asset<Slice_Def_Asset>( "simple_ui_button" );
			current_control->text_align = e_align::left | e_align::vcenter;
			current_control->uses_click_offset = true;
			break;
	}

	current_control->type = control_type;

	assert( current_control );

	current_control->tag = tag;
	current_control->default_width = current_control->get_default_width( current_control->type );
	current_control->default_height = current_control->get_default_height( current_control->type );

	if( current_control->type == e_ui_control_type::panel )
	{
		current_control->default_width = layout_top().w;
		current_control->default_height = layout_top().h;
	}

	if( tag == hash_none )
	{
		// if the control is active and does NOT have a specific tag, auto
		// generate an id for it
		if( current_control->is_active )
		{
			current_control->tag = automatic_id++;
		}
	}

	set_size( { Vec2::defaulted, Vec2::defaulted } );

	return this;
}

UI_Mgr* UI_Mgr::set_text( std::string text )
{
	current_control->text = text;
	return this;
}

UI_Mgr* UI_Mgr::center_control_on_screen()
{
	// only valid for panel controls
	assert( current_control->type == e_ui_control_type::panel );

	Vec2 centered_pos;

	centered_pos.x = ( ui_w - current_control->rc_ui.w ) / 2.f;
	centered_pos.y = ( ui_h - current_control->rc_ui.h ) / 2.f;

	set_pos( centered_pos );

	return this;
}

// looks at the current panel
UI_Mgr* UI_Mgr::adjust_layout_to_client_area()
{
	// only valid for panel controls
	assert( current_control->type == e_ui_control_type::panel );

	current_control->rc_client = compute_client_rect_from_ui_rect( current_control->rc_ui );

	// #layout : NOTE : this doesn't allow for panels within panels since this clears the
	// stack entirely but that can be added in the future if needed
	layout_init( current_control->rc_client );

	return this;
}

UI_Mgr* UI_Mgr::set_text_align( e_align align )
{
	current_control->text_align = align;
	return this;
}

UI_Mgr* UI_Mgr::set_text_color( Color color )
{
	current_control->text_color = color;
	return this;
}

UI_Mgr* UI_Mgr::set_color( Color color )
{
	current_control->primary_color = color;
	return this;
}

UI_Mgr* UI_Mgr::set_image( std::string_view tex_tag )
{
	return set_image( g_engine->find_asset<Texture_Asset>( tex_tag ) );
}

UI_Mgr* UI_Mgr::set_image( const Texture_Asset* tex )
{
	current_control->image = tex;

	return this;
}

UI_Mgr* UI_Mgr::set_func_draw( f_draw_control func_draw_control )
{
	current_control->func_draw_control = func_draw_control;
	return this;
}

UI_Mgr* UI_Mgr::set_slice_def( std::string_view slice_def_name )
{
	current_control->slice_def = nullptr;

	if( !slice_def_name.empty() )
	{
		current_control->slice_def = g_engine->find_asset<Slice_Def_Asset>( slice_def_name );
	}

	current_control->rc_client = compute_client_rect_from_ui_rect( current_control->rc_ui );

	return this;
}

UI_Mgr* UI_Mgr::set_rect( const Rect& rect )
{
	set_pos( { rect.x, rect.y } );
	set_size( { rect.w, rect.h } );

	return this;
}

UI_Mgr* UI_Mgr::set_pos( const Vec2& pos )
{
	current_control->rc_ui.x = pos.x;
	current_control->rc_ui.y = pos.y;

	current_control->rc_client = compute_client_rect_from_ui_rect( current_control->rc_ui );
	current_control->set_pos_called = true;

	return this;
}

UI_Mgr* UI_Mgr::set_size( const Vec2& sz )
{
	if( !fequals( sz.w, Vec2::ignored ) )
	{
		current_control->rc_ui.w =
			fequals( sz.w, Vec2::defaulted )
			? current_control->default_width
			: sz.w;
	}

	if( !fequals( sz.h, Vec2::ignored ) )
	{
		current_control->rc_ui.h =
			fequals( sz.h, Vec2::defaulted )
			? current_control->default_height
			: sz.h;
	}

	current_control->rc_client = compute_client_rect_from_ui_rect( current_control->rc_ui );

	return this;
}

UI_Mgr* UI_Mgr::set_interval( float_t interval )
{
	current_control->interval = interval;
	return this;
}

UI_Mgr* UI_Mgr::set_idx( int32_t idx )
{
	current_control->idx = idx;
	return this;
}

// copies the window rect to the client rect, and then makes adjustments
// to compensate for graphical frames and whatever else would affect the client area.

Rect UI_Mgr::compute_client_rect_from_ui_rect( Rect rc_ui )
{
	auto result = rc_ui;

	if( current_control->slice_def )
	{
		result.x += current_control->slice_def->patches[ (int32_t)e_slice_def_patch::top_left ]->rc.w;
		result.y += current_control->slice_def->patches[ (int32_t)e_slice_def_patch::top_left ]->rc.h;

		result.w -= current_control->slice_def->patches[ (int32_t)e_slice_def_patch::top_left ]->rc.w + current_control->slice_def->patches[ (int32_t)e_slice_def_patch::bottom_right ]->rc.w;
		result.h -= current_control->slice_def->patches[ (int32_t)e_slice_def_patch::top_left ]->rc.h + current_control->slice_def->patches[ (int32_t)e_slice_def_patch::bottom_right ]->rc.h;
	}

	return result;
}

bool UI_Mgr::done()
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

	if( current_control->type == e_ui_control_type::panel )
	{
		// we always want panels to be below anything they contain, so nudge the
		// depth value a little before leaving
		Render::state->z += zdepth_nudge;
	}

	return (int32_t)( result.code & e_im_result::left_clicked );
}

void UI_Mgr::end_static_control()
{
	result.code = e_im_result::none;
	result.click_pos = Vec2::zero;
	result.click_pct = Vec2::zero;

	draw( current_control.get(), false, false );
}

void UI_Mgr::end_active_control()
{
	bool is_hot = ( hot_tag == current_control->tag );
	bool is_hovered = ( hover_tag == current_control->tag );

	// only the topmost scene is allowed to respond to UI input

	if( auto current_scene = g_engine->scene_mgr.current_scene ; current_scene == g_engine->scene_mgr.get_top() )
	{
		Range expanded_tag_range( ( float_t )current_scene->ui_expanded_tag_begin, ( float_t )current_scene->ui_expanded_tag_end );
		if( current_scene->ui_expanded_tag_begin == hash_none or expanded_tag_range.is_within( ( float_t )current_control->tag ) )
		{
			update_im_state( current_control.get(), is_hovered, is_hot );

			draw( current_control.get(), is_hovered, is_hot );

			if( is_hot )
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

void UI_Mgr::update_im_state( UI_Control* control, bool is_hovered, bool is_hot )
{
	control->rc_ui.w =
		fequals( control->rc_ui.w, 0.f )
		? control->default_width
		: control->rc_ui.w;

	control->rc_ui.h =
		fequals( control->rc_ui.h, 0.f )
		? control->default_height
		: control->rc_ui.h;

	result.code = e_im_result::none;

	e_button_state bs_left = g_engine->input_mgr.get_button_state( e_input_id::mouse_button_left );
	e_button_state bs_middle = g_engine->input_mgr.get_button_state( e_input_id::mouse_button_middle );
	e_button_state bs_right = g_engine->input_mgr.get_button_state( e_input_id::mouse_button_right );

	auto rc_ui_offset = control->rc_ui;
	if( is_mouse_inside( rc_ui_offset ) )
	{
		if(
			( bs_left == e_button_state::up and bs_middle == e_button_state::up && bs_right == e_button_state::up )
			or ( bs_left == e_button_state::held and g_ui->hot_tag == control->tag )
			)
		{
			g_ui->hover_tag = control->tag;
		}
		else if( bs_left == e_button_state::pressed )
		{
			g_ui->hot_tag = control->tag;
		}
		else if( bs_left == e_button_state::released )
		{
			if( g_ui->hot_tag == control->tag and g_ui->hover_tag == control->tag )
			{
				result.code |= e_im_result::left_clicked;
				current_callback->on_control_left_clicked( control->tag, result );
			}
			g_ui->hover_tag = g_ui->hot_tag = hash_none;
		}
	}
	else
	{
		if( g_ui->hover_tag == control->tag )
		{
			if( g_ui->hover_tag == control->tag )
			{
				g_ui->hover_tag = hash_none;
			}
		}

		if( bs_left == e_button_state::released and g_ui->hot_tag == control->tag )
		{
			g_ui->hot_tag = hash_none;
		}
	}

	if( g_ui->hover_tag == control->tag )
	{
		result.code |= e_im_result::hovered;
	}
	if( g_ui->hot_tag == control->tag )
	{
		result.code |= e_im_result::hot;
	}

	// client rect position of mouse cursor

	if( (int32_t)(result.code & e_im_result::left_clicked) or control->type == e_ui_control_type::slider )
	{
		// convert mouse location to client rect position inside control

		auto viewport_pos = Coord_System::window_to_viewport_pos( g_engine->input_mgr.mouse_window_pos );
		auto ui_pos = Coord_System::viewport_to_ui_pos( viewport_pos );

		result.click_pos =
			ui_pos
			- Vec2( current_control->rc_ui.x, current_control->rc_ui.y );

		result.click_pct.x = result.click_pos.x / current_control->rc_ui.w;
		result.click_pos.y = result.click_pos.y / current_control->rc_ui.h;

		result.click_pct.x = glm::clamp( result.click_pct.x, 0.f, 1.f );
		result.click_pct.y = glm::clamp( result.click_pct.y, 0.f, 1.f );
	}
}

void UI_Mgr::draw( UI_Control* control, bool is_hovered, bool is_hot )
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

	Rect rc_ui = control->rc_ui;
	rc_ui.x += margins.x / 2.f;
	rc_ui.y += margins.y / 2.f;
	rc_ui.w -= margins.x;
	rc_ui.h -= margins.y;

	Rect rc_client = control->rc_client;
	rc_client.x += margins.x / 2.f;
	rc_client.y += margins.y / 2.f;
	rc_client.w -= margins.x;
	rc_client.h -= margins.y;

	Vec2 clicked_offset = get_click_offset( is_hovered, is_hot );
	Rect rc_ui_offset = rc_ui + clicked_offset;
	Rect rc_client_offset = rc_client + clicked_offset;

	{
		scoped_render_state;

		// if the expanded tag is being cleared, then force all control data to
		// be unexpanded before drawing this frame.
		if( g_engine->scene_mgr.current_scene->flags.clear_expanded_tag_this_frame )
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

Color UI_Mgr::get_adjusted_color( const Color& base_color, bool is_hovered, bool is_hot )
{
	Color color = base_color;

	if( is_hot )
	{
		Color::scale( color, 1.75f );
	}
	else if( is_hovered )
	{
		Color::scale( color, 1.25f );
	}

	return color;
}

UI_Mgr* UI_Mgr::layout_init()
{
	layout_stack.clear();
	layout_push( { 0.f, 0.f, ui_w, ui_h } );
	return this;
}

UI_Mgr* UI_Mgr::layout_init( Rect rc )
{
	layout_stack.clear();
	layout_push( rc );

	return this;
}

UI_Mgr* UI_Mgr::layout_push()
{
	Rect layout = { 0.f, 0.f, ui_w, ui_h };
	layout_stack.push_back( layout );

	return this;
}

UI_Mgr* UI_Mgr::layout_push( Rect rc )
{
	layout_stack.push_back( rc );
	return this;
}

UI_Mgr* UI_Mgr::layout_pop()
{
	assert( !layout_stack.empty() );

	layout_stack.erase( layout_stack.begin() + ( layout_stack.size() - 1 ) );
	return this;
}

Rect UI_Mgr::layout_top()
{
	assert( !layout_stack.empty() );

	return layout_stack.back();
}

Rect* UI_Mgr::layout_top_ptr()
{
	assert( !layout_stack.empty() );

	return &( layout_stack.back() );
}

UI_Mgr* UI_Mgr::cut_top( float_t sz )
{
	set_rect( layout_top_ptr()->cut_top( sz > 0.f ? sz : current_control->default_height + current_control->get_control_inner_margins().y ) );
	return this;

}

UI_Mgr* UI_Mgr::cut_bottom( float_t sz )
{
	set_rect( layout_top_ptr()->cut_bottom( sz > 0.f ? sz : current_control->default_height + current_control->get_control_inner_margins().y ) );
	return this;

}

UI_Mgr* UI_Mgr::cut_left( float_t sz )
{
	set_rect( layout_top_ptr()->cut_left( sz > 0.f ? sz : current_control->default_width + current_control->get_control_inner_margins().x ) );
	return this;

}

UI_Mgr* UI_Mgr::cut_right( float_t sz )
{
	set_rect( layout_top_ptr()->cut_right( sz > 0.f ? sz : current_control->default_width + current_control->get_control_inner_margins().x ) );
	return this;

}

void UI_Mgr::draw_topmost()
{
	// mouse cursor

	if( mouse_cursor and g_engine->window.mouse_mode == e_mouse_mode::custom )
	{
		{
			scoped_render_state;

			Render::state->z = zdepth_topmost;

			auto viewport_pos = Coord_System::window_to_viewport_pos( g_engine->input_mgr.mouse_window_pos );
			auto ui_pos = Coord_System::viewport_to_ui_pos( viewport_pos );

			Render::draw_quad(
				mouse_cursor->texture,
				Vec2( ( ui_pos.x - mouse_cursor->hotspot_offset.x ), ( ui_pos.y - mouse_cursor->hotspot_offset.y ) )
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

Vec2 UI_Mgr::get_click_offset( bool is_hovered, bool is_hot )
{
	if( current_control->uses_click_offset and is_hovered and is_hot )
	{
		return Vec2( 1, 1 );
	}

	return Vec2( 0, 0 );
}

// checks if the mouse cursor is inside the bounds of a rectangle.
// note : both the mouse position and the rectangle need to be in UI space

bool UI_Mgr::is_mouse_inside( Rect& rc )
{
	auto viewport_pos = Coord_System::window_to_viewport_pos( g_engine->input_mgr.mouse_window_pos );
	auto ui_pos = Coord_System::viewport_to_ui_pos( viewport_pos );

	return rc.contains_point( ui_pos );
}

}
