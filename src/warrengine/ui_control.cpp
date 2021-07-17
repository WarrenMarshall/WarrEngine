
#include "master_pch.h"
#include "master_header.h"

namespace war
{

ui_control::ui_control( hash tag )
	: tag( tag )
{
	text_color = make_color( pal::middle );
	primary_color = make_color( pal::middle );
}

void ui_control::draw_slice_def( const rect& rc_ui, bool is_hovered, bool is_hot )
{
	if( slice_def )
	{
		render::state->z += zdepth_nudge;
		render::state->color = g_ui->get_adjusted_color( make_color( pal::darker ), is_hovered, is_hot );

		render::draw_sliced( slice_def, rc_ui );
	}
}

void ui_control::draw_text( const rect& rc_client, const color& color, bool is_hovered, bool is_hot, std::string_view text )
{
	if( text.length() )
	{
		const vec2 pos = rc_client.get_pos_from_alignment( text_align );

		render::state->align = text_align;
		render::state->color = g_ui->get_adjusted_color( color, is_hovered, is_hot );

		render::draw_string( text, pos );
	}
}

void ui_control::draw_texture( const rect& rc, texture_asset* texture, bool is_hovered, bool is_hot )
{
	render::state->z += zdepth_nudge;
	render::state->color = g_ui->get_adjusted_color( make_color( pal::middle ), is_hovered, is_hot );
	render::draw_sprite( texture, rc.get_midpoint() );
}

void ui_control::draw_image( const rect& rc, texture_asset* texture )
{
	render::state->z += zdepth_nudge;
	render::state->color = make_color( color::white );
	render::draw_sprite( texture, rc.get_midpoint() );
}

void ui_control::draw( const rect& rc_ui, const rect& rc_client, bool is_hovered, bool is_hot )
{
}

vec2 ui_control::get_control_inner_margins()
{
	return vec2( 1.f, 2.f );
}

// ----------------------------------------------------------------------------

ui_panel_control::ui_panel_control( hash tag )
	: ui_control( tag )
{
	type = ui_control_type::panel;
	slice_def = g_engine->find_asset<slice_def_asset>( "simple_ui_panel" );
}

void ui_panel_control::draw( const rect& rc_ui, const rect& rc_client, bool is_hovered, bool is_hot )
{
	draw_slice_def( rc_ui, is_hovered, is_hot );
}

float ui_panel_control::get_default_width()
{
	return 100.f;
}

float ui_panel_control::get_default_height()
{
	return 100.f;
}

// ----------------------------------------------------------------------------

ui_caption_control::ui_caption_control( hash tag )
	: ui_control( tag )
{
	type = ui_control_type::caption;
	text_align = align::hcenter | align::vcenter;
	primary_color = make_color( pal::darkest );
}

void ui_caption_control::draw( const rect& rc_ui, const rect& rc_client, bool is_hovered, bool is_hot )
{
	// background color bar

	render::state->color = primary_color;
	render::draw_filled_rect( rc_ui );

	// caption text

	draw_text( rc_client, text_color, is_hovered, is_hot, text );
}


float ui_caption_control::get_default_width()
{
	return 100.f;
}

float ui_caption_control::get_default_height()
{
	return 12.f;
}

// ----------------------------------------------------------------------------

ui_button_control::ui_button_control( hash tag )
	: ui_control( tag )
{
	type = ui_control_type::button;
	is_active = true;

	slice_def = g_engine->find_asset<slice_def_asset>( "simple_ui_button" );
	text_align = align::centered;
}

void ui_button_control::draw( const rect& rc_ui, const rect& rc_client, bool is_hovered, bool is_hot )
{
	draw_slice_def( rc_ui, is_hovered, is_hot );
	draw_text( rc_client, text_color, is_hovered, is_hot, text );
}

float ui_button_control::get_default_width()
{
	return 120.f;
}

float ui_button_control::get_default_height()
{
	return 24.f;
}

// ----------------------------------------------------------------------------

ui_check_control::ui_check_control( hash tag )
	: ui_control( tag )
{
	type = ui_control_type::check;
	is_active = true;
	text_align = align::left | align::vcenter;
}

void ui_check_control::draw( const rect& rc_ui, const rect& rc_client, bool is_hovered, bool is_hot )
{
	texture_asset* texture = g_ui->current_callback->get_texture_for_checkbox( tag );

	draw_slice_def( rc_ui, is_hovered, is_hot );

	rect rc_texture =
		rect(
			rc_client.x,
			rc_client.y,
			texture->rc.w,
			rc_client.h
		);

	draw_texture( rc_texture, texture, is_hovered, is_hot );

	auto rc_text =
		rect(
			rc_client.x + g_ui->current_callback->get_control_padding() + texture->rc.w,
			rc_client.y,
			rc_client.w - g_ui->current_callback->get_control_padding() - texture->rc.w,
			rc_client.h
		);

	draw_text( rc_text, text_color, is_hovered, is_hot, text );
}

float ui_check_control::get_default_width()
{
	return 120.f;
}

float ui_check_control::get_default_height()
{
	return 12.f;
}

// ----------------------------------------------------------------------------

ui_divider_control::ui_divider_control( hash tag )
	: ui_control( tag )
{
	type = ui_control_type::divider;
	slice_def = g_engine->find_asset<slice_def_asset>( "simple_ui_divider" );
}

void ui_divider_control::draw( const rect& rc_ui, const rect& rc_client, bool is_hovered, bool is_hot )
{
	draw_slice_def( rc_ui, false, false );
}

float ui_divider_control::get_default_width()
{
	return 120.f;
}

float ui_divider_control::get_default_height()
{
	return 2.f;
}

vec2 ui_divider_control::get_control_inner_margins()
{
	return { 0.f, 4.f };
}

// ----------------------------------------------------------------------------

ui_spacer_control::ui_spacer_control( hash tag )
	: ui_control( tag )
{
	type = ui_control_type::spacer;
}

void ui_spacer_control::draw( const rect& rc_ui, const rect& rc_client, bool is_hovered, bool is_hot )
{
}

float ui_spacer_control::get_default_width()
{
	return 4.0f;
}

float ui_spacer_control::get_default_height()
{
	return 4.f;
}

vec2 ui_spacer_control::get_control_inner_margins()
{
	return { 0.f, 0.f };
}

// ----------------------------------------------------------------------------

ui_image_control::ui_image_control( hash tag )
	: ui_control( tag )
{
	type = ui_control_type::image;

	g_ui->set_size( { g_ui->current_control->rc_client.w, g_ui->current_control->rc_client.h } );
}

void ui_image_control::draw( const rect& rc_ui, const rect& rc_client, bool is_hovered, bool is_hot )
{
	draw_image( rc_ui, image );
}

float ui_image_control::get_default_width()
{
	return 120.f;
}

float ui_image_control::get_default_height()
{
	return 24.f;
}

// ----------------------------------------------------------------------------

ui_label_control::ui_label_control( hash tag )
	: ui_control( tag )
{
	type = ui_control_type::label;
	text_align = align::left | align::vcenter;
}

void ui_label_control::draw( const rect& rc_ui, const rect& rc_client, bool is_hovered, bool is_hot )
{
	draw_slice_def( rc_ui, false, false );
	draw_text( rc_client, text_color, false, false, text );
}

float ui_label_control::get_default_width()
{
	return 120.f;
}

float ui_label_control::get_default_height()
{
	return 14.f;
}

// ----------------------------------------------------------------------------

ui_slider_control::ui_slider_control( hash tag )
	: ui_control( tag )
{
	type = ui_control_type::slider;
	is_active = true;
	text_align = align::left | align::vcenter;
	slice_def = g_engine->find_asset<slice_def_asset>( "simple_ui_slider_body" );
	uses_click_offset = false;
}

void ui_slider_control::draw( const rect& rc_ui, const rect& rc_client, bool is_hovered, bool is_hot )
{
	draw_slice_def( rc_ui, is_hovered, is_hot );

	render::state->color = g_ui->get_adjusted_color( make_color( pal::middle ), is_hovered, is_hot );

	if( interval )
	{
		draw_slider_tick_marks( rc_ui, rc_client );
	}

	draw_slider_thumb( rc_client );
}

void ui_slider_control::draw_slider_thumb( const rect& rc_client )
{
	// draw the thumb indicator

	auto data = g_ui->current_callback->get_data( tag )->float_value_internal();

	vec2 pos = vec2(
		rc_client.x + ( rc_client.w * data ),
		rc_client.y + ( rc_client.h / 2.f )
	);

	render::draw_sprite( g_engine->find_asset<texture_asset>( "ui_slider_thumb" ), pos );
}

void ui_slider_control::draw_slider_tick_marks( const rect& rc_ui, const rect& rc_client )
{
	if( fequals( interval, 0.f ) )
	{
		return;
	}

	auto tex_tick = g_engine->find_asset<texture_asset>( "ui_slider_tick" );

	vec2 tick_pos = vec2( rc_client.x, rc_ui.y + 5.f );
	auto steps = (int)( 1.f / interval );
	auto stride = rc_client.w * interval;

	for( auto x = 0 ; x <= steps ; ++x )
	{
		render::draw_sprite( tex_tick, tick_pos );
		tick_pos.x += stride;
	}
}

float ui_slider_control::get_default_width()
{
	return 120.f;
}

float ui_slider_control::get_default_height()
{
	return 6.f;
}

vec2 ui_slider_control::get_control_inner_margins()
{
	return vec2( 1.f, 8.f );
}

// ----------------------------------------------------------------------------

ui_text_control::ui_text_control( hash tag )
	: ui_control( tag )
{
	type = ui_control_type::text;
	is_active = true;
	slice_def = g_engine->find_asset<slice_def_asset>( "simple_ui_text" );
	text_align = align::left | align::vcenter;
	uses_click_offset = false;
	can_retain_focus = true;
}

void ui_text_control::draw( const rect& rc_ui, const rect& rc_client, bool is_hovered, bool is_hot )
{
	auto control_data = static_cast<ui_text_control_data*>( g_ui->current_callback->get_data( tag ) );

	// background
	draw_slice_def( rc_ui, is_hovered, is_hot );

	// text
	auto text_from_data = control_data->string_value();
	std::string_view text_being_used;

	if( text_from_data.length() )
	{
		text_being_used = text_from_data;
		draw_text( rc_client + rect( 0, -1, 0, 0 ), text_color, is_hovered, is_hot, text_from_data );
	}
	else
	{
		text_being_used = text;
		draw_text( rc_client + rect( 0, -1, 0, 0 ), make_color( text_color, 0.25f ), is_hovered, is_hot, text );
	}

	if( g_ui->focused.tag == tag )
	{
		if( *g_ui->caret_blink_tween < 0.5f )
		{
			// caret
			vec2 extents = g_engine->pixel_font->get_string_extents( text_being_used.substr( 0, control_data->caret_pos ) );
			auto tex_caret = g_engine->find_asset<texture_asset>( "ui_text_caret" );

			render::state->color = color::white;
			render::draw_sprite( tex_caret,
				{
					rc_client.x + extents.x,
					rc_client.y + ( rc_client.h / 2.f )
				}
			);
		}
	}
}

float ui_text_control::get_default_width()
{
	return 120.f;
}

float ui_text_control::get_default_height()
{
	return 16.f;
}

// ----------------------------------------------------------------------------

ui_radio_control::ui_radio_control( hash tag )
	: ui_control( tag )
{
	type = ui_control_type::radio;
	is_active = true;
	text_align = align::left | align::vcenter;
}

void ui_radio_control::draw( const rect& rc_ui, const rect& rc_client, bool is_hovered, bool is_hot )
{
	texture_asset* texture = g_ui->current_callback->get_texture_for_radio( tag );
	draw_slice_def( rc_ui, is_hovered, is_hot );

	rect rc_texture =
		rect(
			rc_client.x, rc_client.y,
			texture->rc.w,
			rc_client.h
		);

	draw_texture( rc_texture, texture, is_hovered, is_hot );

	rect rc_text =
	{
		rc_client.x + g_ui->current_callback->get_control_padding() + texture->rc.w,
		rc_client.y,
		rc_client.w - g_ui->current_callback->get_control_padding() - texture->rc.w,
		rc_client.h
	};

	draw_text( rc_text, text_color, is_hovered, is_hot, text );
}

float ui_radio_control::get_default_width()
{
	return 120.f;
}

float ui_radio_control::get_default_height()
{
	return 12.f;
}

// ----------------------------------------------------------------------------

ui_progress_control::ui_progress_control( hash tag )
	: ui_control( tag )
{
	type = ui_control_type::progress;
}

void ui_progress_control::draw( const rect& rc_ui, const rect& rc_client, bool is_hovered, bool is_hot )
{
	auto control_data = static_cast<ui_progress_control_data*>( g_ui->current_callback->get_data( tag ) );

	// background
	draw_slice_def( rc_ui, is_hovered, is_hot );

	// progress bar
	float pct = control_data->float_value_internal();

	rect shrunken_rc = rc_client;
	shrunken_rc.shrink( 1.f );
	shrunken_rc.w *= pct;

	render::state->color = make_color( pal::darker );
	render::draw_filled_rect( rc_client );

	render::state->color = make_color( pal::middle );
	render::draw_filled_rect( shrunken_rc );

	if( control_data->draw_percentage_as_text and pct >= 0.01f )
	{
		render::state->color = make_color( pal::brightest );
		render::state->align = align::centered;
		render::draw_string( std::format( "{}%", (int)glm::round( pct * 100.f ) ), rc_client.get_midpoint() );
	}
}

float ui_progress_control::get_default_width()
{
	return 120.f;
}

float ui_progress_control::get_default_height()
{
	return 12.f;
}

// ----------------------------------------------------------------------------

ui_list_control::ui_list_control( hash tag )
	: ui_control( tag )
{
	type = ui_control_type::list;
	text_align = align::left | align::top;
	slice_def = g_engine->find_asset<slice_def_asset>( "simple_ui_list" );
}

void ui_list_control::draw( const rect& rc_ui, const rect& rc_client, bool is_hovered, bool is_hot )
{
	auto control_data = g_ui->current_callback->get_data( tag );

	auto list_control_tag = tag;
	rect list_rc_client = rc_client;
	auto interior_margin = g_ui->current_control->get_control_inner_margins();

	// draw background

	draw_slice_def( rc_ui, is_hovered, is_hot );

	// create a button for each item in the list
	vec2 pos = { list_rc_client.x + interior_margin.x, list_rc_client.y };

	auto num_items = g_ui->current_callback->get_item_count( list_control_tag );
	for( auto idx = 0 ; idx < num_items ; ++idx )
	{
		auto item_str = g_ui->current_callback->get_item_for_idx( list_control_tag, idx );

		render::state->z += zdepth_nudge;
		{
			if( g_ui
				->button_control( list_control_tag + idx + 1000 )
				->set_slice_def( "" )
				->set_pos( pos )
				->set_size( { list_rc_client.w, render::state->font->get_max_height() } )
				->set_text( item_str.data() )
				->set_text_align( align::left | align::vcenter )
				->set_idx( idx )
				->done() )
			{
				// if the item was clicked, that index becomes the current one
				control_data->set_int_value( idx );
			}
		}
		render::state->z -= zdepth_nudge;

		// if this item is currently selected, give it a background highlight
		if( idx == control_data->int_value() )
		{
			render::state->color = make_color( pal::darker );
			rect rc = g_ui->current_control->rc_client;
			rc.x -= interior_margin.x;
			render::draw_filled_rect( rc );
		}

		pos.y += render::state->font->get_max_height();
	}
}

float ui_list_control::get_default_width()
{
	return 120.f;
}

float ui_list_control::get_default_height()
{
	return 24.f;
}

// ----------------------------------------------------------------------------

ui_dropdown_control::ui_dropdown_control( hash tag )
	: ui_control( tag )
{
	type = ui_control_type::dropdown;
	is_active = true;
	slice_def = g_engine->find_asset<slice_def_asset>( "simple_ui_button" );
	text_align = align::left | align::vcenter;
	uses_click_offset = true;
}

void ui_dropdown_control::draw( const rect& rc_ui, const rect& rc_client, bool is_hovered, bool is_hot )
{
	auto control_data = static_cast<ui_dropdown_control_data*>( g_ui->current_callback->get_data( tag ) );
	auto dropdown_control_tag = tag;

	render::state->z += zdepth_nudge;
	render::state->z += zdepth_nudge;

	// draw button

	draw_slice_def( rc_ui, is_hovered, is_hot );

	auto str = g_ui->current_callback->get_item_for_idx( dropdown_control_tag, control_data->int_value() );
	draw_text( rc_client, text_color, is_hovered, is_hot, str );

	auto tex = g_engine->find_asset<texture_asset>( "ui_dropdown_arrow" );

	render::state->color = g_ui->get_adjusted_color( make_color( pal::middle ), is_hovered, is_hot );
	render::draw_sprite( tex,
		{
			rc_client.x + rc_client.w - ( tex->rc.w / 2.f ),
			rc_client.y + ( rc_client.h / 2.f )
		}
	);

	render::state->z -= zdepth_nudge;

	if( g_ui->result.code & im_result::left_clicked )
	{
		control_data->set_expanded( !control_data->is_expanded() );
		g_engine->scenes.current_scene->ui_expanded_tag_begin = control_data->is_expanded() ? dropdown_control_tag : hash_none;
		g_engine->scenes.current_scene->ui_expanded_tag_end = g_engine->scenes.current_scene->ui_expanded_tag_begin;
	}

	if( control_data->is_expanded() )
	{
		auto num_items = g_ui->current_callback->get_item_count( dropdown_control_tag );
		auto item_list_height = num_items * render::state->font->get_max_height();
		auto interior_margin = g_ui->current_control->get_control_inner_margins();

		// take the current win rect, which matches the dropdown button itself,
		// and massage it so it sits below the button and has enough height to
		// hold all the items.

		auto slice_def_dropdown_list = g_engine->find_asset<slice_def_asset>( "simple_ui_dropdown_list" );

		rect item_list_rc = rc_ui;
		item_list_rc.x += interior_margin.x * 2.f;
		item_list_rc.y += rc_ui.h;
		item_list_rc.w -= interior_margin.x * 4.f;
		item_list_rc.h +=
			item_list_height
			+ ( interior_margin.y * 2.f )
			- rc_ui.h
			+ slice_def_dropdown_list->get_top_slice_sz()
			+ slice_def_dropdown_list->get_bottom_slice_sz();

		// draw background

		render::state->color = make_color( pal::darker );
		render::draw_sliced( slice_def_dropdown_list, item_list_rc );

		auto rc_item_list_client = g_ui->compute_client_rect_from_ui_rect( item_list_rc );

		// create a button for each item

		render::state->z += zdepth_nudge;
		{
			scoped_ui_layout( rc_item_list_client );

			for( auto idx = 0 ; idx < num_items ; ++idx )
			{
				auto item_str = g_ui->current_callback->get_item_for_idx( dropdown_control_tag, idx );

				auto extents = rc_client.extents();

				g_engine->scenes.current_scene->ui_expanded_tag_end = dropdown_control_tag + idx + 1;

				if( g_ui
					->button_control( g_engine->scenes.current_scene->ui_expanded_tag_end )
					->set_slice_def( "" )
					->cut_top( render::state->font->get_max_height() )
					->set_size( { item_list_rc.w - ( interior_margin.x * 3.f ), vec2::ignored } )
					->set_text( item_str.data() )
					->set_text_align( align::left | align::vcenter )
					->set_idx( idx )
					->done() )
				{
					// if the item was clicked, that index becomes the current one
					control_data->set_int_value( idx );

					control_data->set_expanded( false );
					g_engine->scenes.current_scene->ui_expanded_tag_begin = g_engine->scenes.current_scene->ui_expanded_tag_end = hash_none;
				}

				// if this item is currently selected, give it a background highlight
				if( idx == control_data->int_value() )
				{
					render::state->z -= zdepth_nudge;
					render::state->color = make_color( pal::darker );
					rect rc = g_ui->current_control->rc_client;
					rc.x -= interior_margin.x * 2.f;
					rc.w -= interior_margin.x * 4.f;
					render::draw_filled_rect( rc );
					render::state->z += zdepth_nudge;
				}
			}
		}
	}
}

float ui_dropdown_control::get_default_width()
{
	return 120.f;
}

float ui_dropdown_control::get_default_height()
{
	return 16.f;
}

}