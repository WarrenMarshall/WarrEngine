
#include "app_header.h"

using namespace war;

scene_ui_callback::scene_ui_callback()
{
	text_data.max_length = 15;
	text_data.valid_char_list = valid_chars_alphanumeric + valid_chars_simple_whitespace + valid_chars_punctuation;

	list_data.set_int_value( 0 );
	list_items.emplace_back( "list item #1" );
	list_items.emplace_back( "list item #2" );
	list_items.emplace_back( "list item #3" );

	dropdown_data.set_int_value( 0 );
	dropdown_items.emplace_back( "drop-down #1" );
	dropdown_items.emplace_back( "drop-down #2" );
	dropdown_items.emplace_back( "drop-down #3" );
	dropdown_items.emplace_back( "drop-down #4" );

	progress_data.draw_percentage_as_text = true;
}

UI_Control_Data* scene_ui_callback::get_data( hash tag )
{
	switch( tag )
	{
		case H( "check_box" ):			return &checkbox_data;
		case H( "progress_bar_01" ):	return &progress_data;
		case H( "slider_01" ):			return &slider_01_data;
		case H( "slider_02" ):			return &slider_02_data;
		case H( "radio_button_01" ):
		case H( "radio_button_02" ):
		case H( "radio_button_03" ):	return &radio_data;
		case H( "edit_text" ):			return &text_data;
		case H( "list_01" ):			return &list_data;
		case H( "dropdown_01" ):		return &dropdown_data;
	}

	return UI_Callback::get_data( tag );
}

size_t scene_ui_callback::get_item_count( hash tag )
{
	switch( tag )
	{
		case H( "list_01" ):
		{
			return list_items.size();
		}

		case H( "dropdown_01" ):
		{
			return dropdown_items.size();
		}
	}

	return UI_Callback::get_item_count( tag );
}

std::string_view scene_ui_callback::get_item_for_idx( hash tag, int idx )
{
	switch( tag )
	{
		case H("list_01"):
		{
			return list_items[ idx ];
		}

		case H( "dropdown_01" ):
		{
			return dropdown_items[ idx ];
		}
	}

	return UI_Callback::get_item_for_idx( tag, idx );
}

void scene_ui_callback::on_value_changed( hash tag )
{
	switch( tag )
	{
		case H( "check_box" ):
		{
			g_engine->window.set_mouse_mode( checkbox_data.bool_value() ? mouse_mode::custom : mouse_mode::os );
		}
		break;
	}

}

// ----------------------------------------------------------------------------

scene_ui::scene_ui()
{
	ui_callback = std::make_unique<scene_ui_callback>();
	flags.blocks_further_drawing = true;
}

void scene_ui::pushed()
{
	scene::pushed();

	viewport_pivot = Vec2::zero;
	g_engine->window.set_mouse_mode( mouse_mode::os );
}

void scene_ui::draw()
{
	draw_tiled_background();
	scene::draw();
	Render::draw_world_axis();
}

// ----------------------------------------------------------------------------

f_decl_draw_control( draw_control_player )
{
	scoped_render_state;

	Render::state->z += zdepth_nudge;
	Render::state->color = make_color( Color::white );
	Render::state->scale = 2.f;

	Render::draw_sprite( control->image, rc_ui.get_midpoint() );
}

// ----------------------------------------------------------------------------

void scene_ui::draw_ui()
{
	auto cb = (scene_ui_callback*)get_ui_callback();

	scene::draw_ui();

	float half_panel_w = 120.f;
	float panel_w = (half_panel_w * 2.f) + 16.f;
	float panel_h = 196.f;

	Rect rc_panel = { 0.f, 0.f, panel_w, panel_h };
	g_ui->layout_init( rc_panel );

	g_ui->panel_control()
		->set_rect( rc_panel )
		->center_control_on_screen()
		->adjust_layout_to_client_area()
		->done();

	g_ui->caption_control()
		->set_text( "User Interface Controls" )
		->done();

	g_ui->spacer_control()->done();

	Rect rc = g_ui->layout_top();

	// left side

	{
		auto rc_left = rc.cut_left( 0.5f );
		rc_left.w -= 4.f;
		scoped_ui_layout( rc_left );

		if( g_ui->button_control()->set_text( "push_button" )->done() )
		{
			g_engine->show_msg_box( "You clicked the button! This is the built-in engine message box." );
		}

		g_ui->check_control( H("check_box") )
			->set_text( "check_box" )
			->done();

		g_ui->divider_control()->done();

		cb->progress_data.set_float_value( cb->slider_01_data.float_value() );

		g_ui->progress_control( H( "progress_bar_01" ) )
			->done();

		g_ui->slider_control( H( "slider_01" ) )
			->done();

		g_ui->divider_control()->done();

		g_ui->label_control()
			->set_text( "label" )
			->done();

		g_ui->slider_control( H( "slider_02" ) )
			->set_size( { half_panel_w, Vec2::ignored } )
			->set_interval( 0.2f )
			->done();

		g_ui->divider_control()->done();

		g_ui->radio_control( H( "radio_button_01" ) )
			->set_text( "radio_button : 0" )
			->set_idx( 0 )
			->done();

		g_ui->radio_control( H( "radio_button_02" ) )
			->set_text( "radio_button : 1" )
			->set_idx( 1 )
			->done();

		g_ui->radio_control( H( "radio_button_03" ) )
			->set_text( "radio_button : 2" )
			->set_idx( 2 )
			->done();
	}

	// right side
	{
		auto rc_right = rc;
		scoped_ui_layout( rc_right );

		g_ui
			->text_control( H( "edit_text" ) )
			->set_text( "type here" )
			->done();

		g_ui->divider_control()->done();

		g_ui
			->list_control( H( "list_01" ) )
			->cut_top( Render::state->font->get_max_height() * 6 )
			->done();

		g_ui
			->dropdown_control( H( "dropdown_01" ) )
			->set_text( "drop-down" )
			->done();

		g_ui->divider_control()->done();

		g_ui
			->image_control()
			->set_image( "anim_player_run" )
			->cut_top( g_ui->current_control->image->height() * 3.f )
			->set_func_draw( draw_control_player )
			->done();
	}
}
