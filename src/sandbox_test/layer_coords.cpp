
#include "app_header.h"

// ----------------------------------------------------------------------------

void layer_coords_callback::on_left_clicked( const w_imgui_control& control, const w_imgui_result& result )
{
	w_imgui_callback::on_left_clicked( control, result );

// 	switch( control.tag )
// 	{
// 		case H( "push_button_01" ):
// 		{
// 			UI->show_msg_box( "You clicked the button!" );
// 		}
// 		break;
// 	}
}

void layer_coords_callback::on_motion( const w_imgui_control& control, const w_imgui_result& result )
{
	w_imgui_callback::on_motion( control, result );

	auto layer = static_cast<layer_coords*>( IMGUI->current_layer );

// 	switch( control.tag )
// 	{
// 		case H( "slider_01" ):
// 		{
// 		}
// 		break;
// 	}
}

// ----------------------------------------------------------------------------

layer_coords::layer_coords()
{
	draws_completely_solid = true;
}

void layer_coords::push()
{
	engine->window->set_mouse_mode( mouse_mode::custom );

	{
		add_camera();
	}

	{
		auto e = add_entity<w_entity>();
		e->tag = H( "crosshair" );
		auto ec = e->add_component<ec_sprite>();
		ec->init( "tex_crosshair" );
	}

}

void layer_coords::draw()
{
	{
		scoped_render_state;
		render_state.color = w_color::dark_teal;

		w_render::draw_tiled( a_texture::find( "engine_tile_background_stripe" ), w_rect( -viewport_hw, -viewport_hh, viewport_w, viewport_h ) );
	}

	w_layer::draw();

	w_render::draw_world_axis();
}

void layer_coords::draw_ui()
{
	w_layer::draw_ui();

/*
	{
		scoped_render_state;

		render_state =
		{
			.color = w_color( 0.0f, 0.0f, 0.0f, 0.25f )
		};

		w_render::draw_filled_rectangle( { 8, 32, 168, 104 } );

		render_state =
		{
			.color = w_color::teal
		};

		float xpos = 12.0f, ypos = 36.0f;

		w_render::draw_string( "Mouse Position", { xpos, ypos } );
		ypos += engine->pixel_font->font_def->max_height;

		w_vec2 mouse_pos = INPUT->mouse_window_pos;

		w_render::draw_string(
			fmt::format( "Window: {}, {}", (int)mouse_pos.x, (int)mouse_pos.y ),
			{ xpos, ypos += engine->pixel_font->font_def->max_height }
		);

		auto vpos = w_coord::window_to_virtual( mouse_pos );
		w_render::draw_string(
			fmt::format( "window_to_virtual: {}, {}", (int)vpos.x, (int)vpos.y ),
			{ xpos, ypos += engine->pixel_font->font_def->max_height }
		);

		auto uipos = w_coord::virtual_to_ui( vpos );
		w_render::draw_string(
			fmt::format( "virtual_to_ui: {}, {}", (int)uipos.x, (int)uipos.y ),
			{ xpos, ypos += engine->pixel_font->font_def->max_height }
		);

		auto wpos = w_coord::virtual_to_world( vpos, get_camera() );
		w_render::draw_string(
			fmt::format( "virtual_to_world: {}, {}", (int)wpos.x, (int)wpos.y ),
			{ xpos, ypos += engine->pixel_font->font_def->max_height }
		);

		ypos += engine->pixel_font->font_def->max_height;

		vpos = w_coord::world_to_virtual( wpos, get_camera() );
		w_render::draw_string(
			fmt::format( "world_to_virtual: {}, {}", (int)vpos.x, (int)vpos.y ),
			{ xpos, ypos += engine->pixel_font->font_def->max_height }
		);

		uipos = w_coord::world_to_ui( wpos, get_camera() );
		w_render::draw_string(
			fmt::format( "world_to_ui: {}, {}", (int)uipos.x, (int)uipos.y ),
			{ xpos, ypos += engine->pixel_font->font_def->max_height }
		);

		render_state.color = w_color::yellow;
		w_render::draw_string( fmt::format( "W: {},{}", (int)wpos.x, (int)wpos.y ), uipos + w_vec2( 8, 2 ) );
		w_render::draw_string( fmt::format( "V: {},{}", (int)vpos.x, (int)vpos.y ), uipos + w_vec2( 8, 10 ) );
		w_render::draw_string( fmt::format( "U: {},{}", (int)uipos.x, (int)uipos.y ), uipos + w_vec2( 8, 18 ) );
	}
*/

	{
		scoped_render_state;

		render_state =
		{
			.align = align::centered,
			.color = w_color::white,
			.scale = 2.0f
		};

		w_render::draw_string( "Coordinate Systems", { ui_w / 2.0f, 16.0f } );

		render_state =
		{
			.align = align::hcenter,
			.color = w_color::light_grey,
			.scale = 1.0f
		};

		w_render::draw_string( "R_DRAG - move/rotate camera", w_pos( ui_hw, ui_h - 12.0f ) );
	}

	// label next to the crosshair
	{
		auto e = find_entity( H( "crosshair" ) );

		auto vpos = w_coord::world_to_viewport_pos( e->get_transform()->pos, get_camera() );
		auto uipos = w_coord::viewport_to_ui_pos( vpos );

		scoped_render_state;
		render_state.align = align::vcenter;
		render_state.color = w_color::orange;
		render_state.color.a = 0.75f;
		w_render::draw_string( "<< Your crosshair", uipos + w_vec2( 8.0f, 0.0f ) );
	}
}

w_imgui_callback* layer_coords::get_imgui_callback()
{
	return &imgui_callback;
}

bool layer_coords::on_input_motion( const w_input_event* evt )
{
	if( evt->input_id == input_id::mouse )
	{
		if( INPUT->get_button_state( input_id::mouse_button_left ) == button_state::held )
		{
			auto e = find_entity( H( "crosshair" ) );

			if( evt->control_down )
			{
				e->get_transform()->add_angle_delta( w_coord::window_to_viewport_vec( evt->delta ).x );
			}
			else
			{
				auto viewport_pos = w_coord::window_to_viewport_pos( engine->input->mouse_window_pos );
				auto world_pos = w_coord::viewport_to_world_pos( viewport_pos, get_camera() );
				e->get_transform()->set_pos( world_pos );
			}
		}

		if( INPUT->get_button_state( input_id::mouse_button_right ) == button_state::held )
		{
			if( evt->control_down )
			{
				get_camera()->get_transform()->add_angle_delta( w_coord::window_to_viewport_vec( evt->delta ).x );
			}
			else
			{
				get_camera()->get_transform()->add_pos_delta( w_coord::window_to_viewport_vec( evt->delta ) );
			}

			return true;
		}
	}

	return false;
}
