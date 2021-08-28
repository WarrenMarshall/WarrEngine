
#include "app_header.h"

using namespace war;

// ----------------------------------------------------------------------------

scene_angle_dirs::scene_angle_dirs()
{
	flags.blocks_further_drawing = true;
}

void scene_angle_dirs::pushed()
{
	scene::pushed();

	g_engine->window.set_mouse_mode( mouse_mode::os );
}

void scene_angle_dirs::draw()
{
	draw_tiled_background();
	scene::draw();
	Render::draw_world_axis();

//	scoped_render_state;

	// circle at world origin
	Render::state->color = make_color( pal::brighter );
	Render::draw_filled_circle( Vec2::zero, 6.f );

	// circle at marker position
	Render::state->color = make_color( pal::middle );
	Render::draw_filled_circle( marker_pos, 6.f );

	// connecting line
	Render::state->color = make_color( pal::middle );
	Render::draw_line( Vec2::zero, marker_pos );
}

void scene_angle_dirs::draw_ui()
{
	scene::draw_ui();
	draw_title( "Angles / Directions" );

	scoped_render_state;

	// label next to the marker
	if( marker_pos != Vec2::zero )
	{
		auto ui_pos = Coord_System::world_to_ui_pos( marker_pos + Vec2( 16.f, 0.f ) );

		Render::state->align = align::vcenter;
		Render::state->color = make_color( pal::brightest );

		auto angle = Vec2::clamped_angle_from_dir( Vec2::normalize( marker_pos ) );
		auto dir = Vec2::dir_from_angle( angle );

		const std::vector<std::string> info_lines =
		{
			std::format( "Angle : {:.0f} d", angle ),
			std::format( "Dir   : {:.1f}, {:.1f}", dir.x, dir.y )
		};

		Render::draw_string( info_lines, ui_pos );
	}
}

bool scene_angle_dirs::on_input_motion( const Input_Event* evt )
{
	switch( evt->input_id )
	{
		case input_id::mouse:
		{
			if( g_engine->input.is_button_held( input_id::mouse_button_left ) )
			{
				marker_pos = Coord_System::window_to_world_pos( evt->mouse_pos );
				return true;
			}
		}
		break;
	}

	return false;
}
