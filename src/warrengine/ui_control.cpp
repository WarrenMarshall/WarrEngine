
#include "master_pch.h"
#include "master_header.h"

namespace war
{

UI_Control::UI_Control( hash tag )
	: tag( tag )
{
	text_color = make_color( pal::middle );
	primary_color = make_color( pal::middle );
}

void UI_Control::draw_slice_def( const Rect& rc_ui, bool is_hovered, bool is_hot )
{
	if( slice_def )
	{
		Render::state->z += zdepth_nudge;
		Render::state->color = g_ui->get_adjusted_color( make_color( pal::darker ), is_hovered, is_hot );

		Render::draw_sliced( slice_def, rc_ui );
	}
}

void UI_Control::draw_text( const Rect& rc_client, const Color& color, bool is_hovered, bool is_hot, const std::string& text )
{
	if( !text.empty() )
	{
		const Vec2 pos = rc_client.get_pos_from_alignment( text_align );

		Render::state->align = text_align;
		Render::state->color = g_ui->get_adjusted_color( color, is_hovered, is_hot );

		Render::draw_string( text, pos );
	}
}

void UI_Control::draw_texture( const Rect& rc, Texture_Asset* texture, bool is_hovered, bool is_hot )
{
	Render::state->z += zdepth_nudge;
	Render::state->color = g_ui->get_adjusted_color( make_color( pal::middle ), is_hovered, is_hot );
	Render::draw_sprite( texture, rc.get_midpoint() );
}

void UI_Control::draw_image( const Rect& rc, Texture_Asset* texture )
{
	Render::state->z += zdepth_nudge;
	Render::state->color = make_color( Color::white );
	Render::draw_sprite( texture, rc.get_midpoint() );
}

void UI_Control::draw( const Rect& rc_ui, const Rect& rc_client, bool is_hovered, bool is_hot )
{
}

Vec2 UI_Control::get_control_inner_margins()
{
	return Vec2( 1.f, 2.f );
}

// ----------------------------------------------------------------------------

UI_Panel_Control::UI_Panel_Control( hash tag )
	: UI_Control( tag )
{
	type = ui_control_type::panel;
	slice_def = g_engine->find_asset<Slide_Def_Asset>( "simple_ui_panel" );
}

void UI_Panel_Control::draw( const Rect& rc_ui, const Rect& rc_client, bool is_hovered, bool is_hot )
{
	draw_slice_def( rc_ui, is_hovered, is_hot );
}

float UI_Panel_Control::get_default_width()
{
	return 100.f;
}

float UI_Panel_Control::get_default_height()
{
	return 100.f;
}

// ----------------------------------------------------------------------------

UI_Caption_Control::UI_Caption_Control( hash tag )
	: UI_Control( tag )
{
	type = ui_control_type::caption;
	text_align = align::hcenter | align::vcenter;
	primary_color = make_color( pal::darkest );
}

void UI_Caption_Control::draw( const Rect& rc_ui, const Rect& rc_client, bool is_hovered, bool is_hot )
{
	// background color bar

	Render::state->color = primary_color;
	Render::draw_filled_rect( rc_ui );

	// caption text

	draw_text( rc_client, text_color, is_hovered, is_hot, text );
}


float UI_Caption_Control::get_default_width()
{
	return 100.f;
}

float UI_Caption_Control::get_default_height()
{
	return 12.f;
}

// ----------------------------------------------------------------------------

UI_Button_Control::UI_Button_Control( hash tag )
	: UI_Control( tag )
{
	type = ui_control_type::button;
	is_active = true;

	slice_def = g_engine->find_asset<Slide_Def_Asset>( "simple_ui_button" );
	text_align = align::centered;
}

void UI_Button_Control::draw( const Rect& rc_ui, const Rect& rc_client, bool is_hovered, bool is_hot )
{
	draw_slice_def( rc_ui, is_hovered, is_hot );
	draw_text( rc_client, text_color, is_hovered, is_hot, text );
}

float UI_Button_Control::get_default_width()
{
	return 120.f;
}

float UI_Button_Control::get_default_height()
{
	return 24.f;
}

// ----------------------------------------------------------------------------

UI_Check_Control::UI_Check_Control( hash tag )
	: UI_Control( tag )
{
	type = ui_control_type::check;
	is_active = true;
	text_align = align::left | align::vcenter;
}

void UI_Check_Control::draw( const Rect& rc_ui, const Rect& rc_client, bool is_hovered, bool is_hot )
{
	Texture_Asset* texture = g_ui->current_callback->get_texture_for_checkbox( tag );

	draw_slice_def( rc_ui, is_hovered, is_hot );

	Rect rc_texture =
		Rect(
			rc_client.x,
			rc_client.y,
			texture->rc.w,
			rc_client.h
		);

	draw_texture( rc_texture, texture, is_hovered, is_hot );

	auto rc_text =
		Rect(
			rc_client.x + g_ui->current_callback->get_control_padding() + texture->rc.w,
			rc_client.y,
			rc_client.w - g_ui->current_callback->get_control_padding() - texture->rc.w,
			rc_client.h
		);

	draw_text( rc_text, text_color, is_hovered, is_hot, text );
}

float UI_Check_Control::get_default_width()
{
	return 120.f;
}

float UI_Check_Control::get_default_height()
{
	return 12.f;
}

// ----------------------------------------------------------------------------

UI_Divider_Control::UI_Divider_Control( hash tag )
	: UI_Control( tag )
{
	type = ui_control_type::divider;
	slice_def = g_engine->find_asset<Slide_Def_Asset>( "simple_ui_divider" );
}

void UI_Divider_Control::draw( const Rect& rc_ui, const Rect& rc_client, bool is_hovered, bool is_hot )
{
	draw_slice_def( rc_ui, false, false );
}

float UI_Divider_Control::get_default_width()
{
	return 120.f;
}

float UI_Divider_Control::get_default_height()
{
	return 2.f;
}

Vec2 UI_Divider_Control::get_control_inner_margins()
{
	return { 0.f, 4.f };
}

// ----------------------------------------------------------------------------

UI_Spacer_Control::UI_Spacer_Control( hash tag )
	: UI_Control( tag )
{
	type = ui_control_type::spacer;
}

void UI_Spacer_Control::draw( const Rect& rc_ui, const Rect& rc_client, bool is_hovered, bool is_hot )
{
}

float UI_Spacer_Control::get_default_width()
{
	return 4.f;
}

float UI_Spacer_Control::get_default_height()
{
	return 4.f;
}

Vec2 UI_Spacer_Control::get_control_inner_margins()
{
	return { 0.f, 0.f };
}

// ----------------------------------------------------------------------------

UI_Image_Control::UI_Image_Control( hash tag )
	: UI_Control( tag )
{
	type = ui_control_type::image;
}

void UI_Image_Control::draw( const Rect& rc_ui, const Rect& rc_client, bool is_hovered, bool is_hot )
{
	draw_image( rc_ui, image );
}

float UI_Image_Control::get_default_width()
{
	return 120.f;
}

float UI_Image_Control::get_default_height()
{
	return 24.f;
}

// ----------------------------------------------------------------------------

UI_Label_Control::UI_Label_Control( hash tag )
	: UI_Control( tag )
{
	type = ui_control_type::label;
	text_align = align::left | align::vcenter;
}

void UI_Label_Control::draw( const Rect& rc_ui, const Rect& rc_client, bool is_hovered, bool is_hot )
{
	draw_slice_def( rc_ui, false, false );
	draw_text( rc_client, text_color, false, false, text );
}

float UI_Label_Control::get_default_width()
{
	return 120.f;
}

float UI_Label_Control::get_default_height()
{
	return 14.f;
}

// ----------------------------------------------------------------------------

UI_Slider_Control::UI_Slider_Control( hash tag )
	: UI_Control( tag )
{
	type = ui_control_type::slider;
	is_active = true;
	text_align = align::left | align::vcenter;
	slice_def = g_engine->find_asset<Slide_Def_Asset>( "simple_ui_slider_body" );
	uses_click_offset = false;
}

void UI_Slider_Control::draw( const Rect& rc_ui, const Rect& rc_client, bool is_hovered, bool is_hot )
{
	draw_slice_def( rc_ui, is_hovered, is_hot );

	Render::state->color = g_ui->get_adjusted_color( make_color( pal::middle ), is_hovered, is_hot );

	if( interval )
	{
		draw_slider_tick_marks( rc_ui, rc_client );
	}

	draw_slider_thumb( rc_client );
}

void UI_Slider_Control::draw_slider_thumb( const Rect& rc_client )
{
	// draw the thumb indicator

	auto data = g_ui->current_callback->get_data( tag )->float_value_internal();

	Vec2 pos = Vec2(
		rc_client.x + ( rc_client.w * data ),
		rc_client.y + ( rc_client.h / 2.f )
	);

	Render::draw_sprite( g_engine->find_asset<Texture_Asset>( "ui_slider_thumb" ), pos );
}

void UI_Slider_Control::draw_slider_tick_marks( const Rect& rc_ui, const Rect& rc_client )
{
	if( fequals( interval, 0.f ) )
	{
		return;
	}

	auto tex_tick = g_engine->find_asset<Texture_Asset>( "ui_slider_tick" );

	Vec2 tick_pos = Vec2( rc_client.x, rc_ui.y + 5.f );
	auto steps = (int)( 1.f / interval );
	auto stride = rc_client.w * interval;

	for( auto x = 0 ; x <= steps ; ++x )
	{
		Render::draw_sprite( tex_tick, tick_pos );
		tick_pos.x += stride;
	}
}

float UI_Slider_Control::get_default_width()
{
	return 120.f;
}

float UI_Slider_Control::get_default_height()
{
	return 6.f;
}

Vec2 UI_Slider_Control::get_control_inner_margins()
{
	return Vec2( 1.f, 8.f );
}

// ----------------------------------------------------------------------------

UI_Text_Control::UI_Text_Control( hash tag )
	: UI_Control( tag )
{
	type = ui_control_type::text;
	is_active = true;
	slice_def = g_engine->find_asset<Slide_Def_Asset>( "simple_ui_text" );
	text_align = align::left | align::vcenter;
	uses_click_offset = false;
	can_retain_focus = true;
}

void UI_Text_Control::draw( const Rect& rc_ui, const Rect& rc_client, bool is_hovered, bool is_hot )
{
	auto control_data = static_cast<UI_Text_Control_Data*>( g_ui->current_callback->get_data( tag ) );

	// background
	draw_slice_def( rc_ui, is_hovered, is_hot );

	// text
	auto text_from_data = control_data->string_value();
	std::string_view text_being_used;

	if( text_from_data.length() )
	{
		text_being_used = text_from_data;
		draw_text( rc_client + Rect( 0, -1, 0, 0 ), text_color, is_hovered, is_hot, text_from_data );
	}
	else
	{
		text_being_used = text;
		draw_text( rc_client + Rect( 0, -1, 0, 0 ), make_color( text_color, 0.25f ), is_hovered, is_hot, text );
	}

	if( g_ui->focused.tag == tag )
	{
		if( *g_ui->caret_blink_tween < 0.5f )
		{
			// caret
			Vec2 extents = g_engine->pixel_font->get_string_extents( text_being_used.substr( 0, control_data->caret_pos ) );
			auto tex_caret = g_engine->find_asset<Texture_Asset>( "ui_text_caret" );

			Render::state->color = Color::white;
			Render::draw_sprite( tex_caret,
				{
					rc_client.x + extents.x,
					rc_client.y + ( rc_client.h / 2.f )
				}
			);
		}
	}
}

float UI_Text_Control::get_default_width()
{
	return 120.f;
}

float UI_Text_Control::get_default_height()
{
	return 16.f;
}

// ----------------------------------------------------------------------------

UI_Radio_Control::UI_Radio_Control( hash tag )
	: UI_Control( tag )
{
	type = ui_control_type::radio;
	is_active = true;
	text_align = align::left | align::vcenter;
}

void UI_Radio_Control::draw( const Rect& rc_ui, const Rect& rc_client, bool is_hovered, bool is_hot )
{
	Texture_Asset* texture = g_ui->current_callback->get_texture_for_radio( tag );
	draw_slice_def( rc_ui, is_hovered, is_hot );

	Rect rc_texture =
		Rect(
			rc_client.x, rc_client.y,
			texture->rc.w,
			rc_client.h
		);

	draw_texture( rc_texture, texture, is_hovered, is_hot );

	Rect rc_text =
	{
		rc_client.x + g_ui->current_callback->get_control_padding() + texture->rc.w,
		rc_client.y,
		rc_client.w - g_ui->current_callback->get_control_padding() - texture->rc.w,
		rc_client.h
	};

	draw_text( rc_text, text_color, is_hovered, is_hot, text );
}

float UI_Radio_Control::get_default_width()
{
	return 120.f;
}

float UI_Radio_Control::get_default_height()
{
	return 12.f;
}

// ----------------------------------------------------------------------------

UI_Progress_Control::UI_Progress_Control( hash tag )
	: UI_Control( tag )
{
	type = ui_control_type::progress;
}

void UI_Progress_Control::draw( const Rect& rc_ui, const Rect& rc_client, bool is_hovered, bool is_hot )
{
	auto control_data = static_cast<UI_Progress_Control_Data*>( g_ui->current_callback->get_data( tag ) );

	// background
	draw_slice_def( rc_ui, is_hovered, is_hot );

	// progress bar
	float pct = control_data->float_value_internal();

	Rect shrunken_rc = rc_client;
	shrunken_rc.shrink( 1.f );
	shrunken_rc.w *= pct;

	Render::state->color = make_color( pal::darker );
	Render::draw_filled_rect( rc_client );

	Render::state->color = make_color( pal::middle );
	Render::draw_filled_rect( shrunken_rc );

	if( control_data->draw_percentage_as_text and pct >= 0.01f )
	{
		Render::state->color = make_color( pal::brightest );
		Render::state->align = align::centered;
		Render::draw_string( std::format( "{}%", (int)glm::round( pct * 100.f ) ), rc_client.get_midpoint() );
	}
}

float UI_Progress_Control::get_default_width()
{
	return 120.f;
}

float UI_Progress_Control::get_default_height()
{
	return 12.f;
}

// ----------------------------------------------------------------------------

UI_List_Control::UI_List_Control( hash tag )
	: UI_Control( tag )
{
	type = ui_control_type::list;
	text_align = align::left | align::top;
	slice_def = g_engine->find_asset<Slide_Def_Asset>( "simple_ui_list" );
}

void UI_List_Control::draw( const Rect& rc_ui, const Rect& rc_client, bool is_hovered, bool is_hot )
{
	auto control_data = g_ui->current_callback->get_data( tag );

	auto list_control_tag = tag;
	Rect list_rc_client = rc_client;
	auto interior_margin = g_ui->current_control->get_control_inner_margins();

	// draw background

	draw_slice_def( rc_ui, is_hovered, is_hot );

	// create a button for each item in the list
	Vec2 pos = { list_rc_client.x + interior_margin.x, list_rc_client.y };

	auto num_items = g_ui->current_callback->get_item_count( list_control_tag );
	for( auto idx = 0 ; idx < num_items ; ++idx )
	{
		auto item_str = g_ui->current_callback->get_item_for_idx( list_control_tag, idx );

		Render::state->z += zdepth_nudge;
		{
			if( g_ui
				->button_control( list_control_tag + idx + 1000 )
				->set_slice_def( "" )
				->set_pos( pos )
				->set_size( { list_rc_client.w, Render::state->font->get_max_height() } )
				->set_text( item_str.data() )
				->set_text_align( align::left | align::vcenter )
				->set_idx( idx )
				->done() )
			{
				// if the item was clicked, that index becomes the current one
				control_data->set_int_value( idx );
			}
		}
		Render::state->z -= zdepth_nudge;

		// if this item is currently selected, give it a background highlight
		if( idx == control_data->int_value() )
		{
			Render::state->color = make_color( pal::darker );
			Rect rc = g_ui->current_control->rc_client;
			rc.x -= interior_margin.x;
			Render::draw_filled_rect( rc );
		}

		pos.y += Render::state->font->get_max_height();
	}
}

float UI_List_Control::get_default_width()
{
	return 120.f;
}

float UI_List_Control::get_default_height()
{
	return 24.f;
}

// ----------------------------------------------------------------------------

UI_Dropdown_Control::UI_Dropdown_Control( hash tag )
	: UI_Control( tag )
{
	type = ui_control_type::dropdown;
	is_active = true;
	slice_def = g_engine->find_asset<Slide_Def_Asset>( "simple_ui_button" );
	text_align = align::left | align::vcenter;
	uses_click_offset = true;
}

void UI_Dropdown_Control::draw( const Rect& rc_ui, const Rect& rc_client, bool is_hovered, bool is_hot )
{
	auto control_data = static_cast<UI_Dropdown_Control_Data*>( g_ui->current_callback->get_data( tag ) );
	auto dropdown_control_tag = tag;

	Render::state->z += zdepth_nudge;
	Render::state->z += zdepth_nudge;

	// draw button

	draw_slice_def( rc_ui, is_hovered, is_hot );

	auto str = g_ui->current_callback->get_item_for_idx( dropdown_control_tag, control_data->int_value() );
	draw_text( rc_client, text_color, is_hovered, is_hot, str.data() );

	auto tex = g_engine->find_asset<Texture_Asset>( "ui_dropdown_arrow" );

	Render::state->color = g_ui->get_adjusted_color( make_color( pal::middle ), is_hovered, is_hot );
	Render::draw_sprite( tex,
		{
			rc_client.x + rc_client.w - ( tex->rc.w / 2.f ),
			rc_client.y + ( rc_client.h / 2.f )
		}
	);

	Render::state->z -= zdepth_nudge;

	if( g_ui->result.code & im_result::left_clicked )
	{
		control_data->set_expanded( !control_data->is_expanded() );
		g_engine->scenes.current_scene->ui_expanded_tag_begin = control_data->is_expanded() ? dropdown_control_tag : hash_none;
		g_engine->scenes.current_scene->ui_expanded_tag_end = g_engine->scenes.current_scene->ui_expanded_tag_begin;
	}

	if( control_data->is_expanded() )
	{
		auto num_items = g_ui->current_callback->get_item_count( dropdown_control_tag );
		auto item_list_height = num_items * Render::state->font->get_max_height();
		auto interior_margin = g_ui->current_control->get_control_inner_margins();

		// take the current win rect, which matches the dropdown button itself,
		// and massage it so it sits below the button and has enough height to
		// hold all the items.

		auto slice_def_dropdown_list = g_engine->find_asset<Slide_Def_Asset>( "simple_ui_dropdown_list" );

		Rect item_list_rc = rc_ui;
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

		Render::state->color = make_color( pal::darker );
		Render::draw_sliced( slice_def_dropdown_list, item_list_rc );

		auto rc_item_list_client = g_ui->compute_client_rect_from_ui_rect( item_list_rc );

		// create a button for each item

		Render::state->z += zdepth_nudge;
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
					->cut_top( Render::state->font->get_max_height() )
					->set_size( { item_list_rc.w - ( interior_margin.x * 3.f ), Vec2::ignored } )
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
					Render::state->z -= zdepth_nudge;
					Render::state->color = make_color( pal::darker );
					Rect rc = g_ui->current_control->rc_client;
					rc.x -= interior_margin.x * 2.f;
					rc.w -= interior_margin.x * 4.f;
					Render::draw_filled_rect( rc );
					Render::state->z += zdepth_nudge;
				}
			}
		}
	}
}

float UI_Dropdown_Control::get_default_width()
{
	return 120.f;
}

float UI_Dropdown_Control::get_default_height()
{
	return 16.f;
}

}