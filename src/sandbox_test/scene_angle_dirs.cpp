
#include "app_header.h"

using namespace war;

// ----------------------------------------------------------------------------

scene_angle_dirs::scene_angle_dirs()
{
	flags.blocks_further_drawing = true;
}

void scene_angle_dirs::pushed()
{
	g_engine->window.set_mouse_mode( mouse_mode::os );
}

void scene_angle_dirs::draw()
{
	draw_tiled_background();
	scene::draw();
	render::draw_world_axis();

//	scoped_render_state;

	// circle at world origin
	render::state->color = make_color( pal::brighter );
	render::draw_filled_circle( vec2::zero, 6.f );

	// circle at marker position
	render::state->color = make_color( pal::middle );
	render::draw_filled_circle( marker_pos, 6.f );

	// connecting line
	render::state->color = make_color( pal::middle );
	render::draw_line( vec2::zero, marker_pos );
}

void scene_angle_dirs::draw_ui()
{
	scene::draw_ui();
	draw_title( "Angles / Directions" );

	scoped_render_state;

	// label next to the marker
	if( marker_pos != vec2::zero )
	{
		auto ui_pos = coord_system::world_to_ui_pos( marker_pos + vec2( 16.f, 0.f ) );

		render::state->align = align::vcenter;
		render::state->color = make_color( pal::brightest );

		auto angle = vec2::clamped_angle_from_dir( vec2::normalize( marker_pos ) );
		auto dir = vec2::dir_from_angle( angle );

		const std::vector<std::string> info_lines =
		{
			std::format( "Angle : {:.0f} d", angle ),
			std::format( "Dir   : {:.1f}, {:.1f}", dir.x, dir.y )
		};

		render::draw_string( info_lines, ui_pos );
	}
}

bool scene_angle_dirs::on_input_motion( const input_event* evt )
{
	switch( evt->input_id )
	{
		case input_id::mouse:
		{
			if( g_engine->input.is_button_held( input_id::mouse_button_left ) )
			{
				marker_pos = coord_system::window_to_world_pos( evt->mouse_pos );
				return true;
			}
		}
		break;
	}

	return false;
}
