
#include "app_header.h"

using namespace war;

// ----------------------------------------------------------------------------

Scene_Angle_Dirs::Scene_Angle_Dirs()
{
	flags.blocks_further_drawing = true;
	flags.blocks_further_update = true;
}

void Scene_Angle_Dirs::draw()
{
	draw_tiled_background();
	Scene::draw();
	Render::draw_world_axis();

	// circle at world origin
	Render::state->color = make_color( e_pal::lighter );
	Render::draw_filled_circle( Vec2::zero, 6.f );

	// marker
	//
	// circle at marker position
	Render::state->color = make_color( e_pal::middle );
	Render::draw_filled_circle( marker_pos, 6.f );

	// connecting line
	Render::state->color = make_color( e_pal::middle );
	Render::draw_line( Vec2::zero, marker_pos );

	// marker2
	//
	// circle at marker position
	Render::state->color = make_color( e_pal::lighter );
	Render::draw_filled_circle( marker_pos2, 6.f );

	// connecting line
	Render::state->color = make_color( e_pal::lighter );
	Render::draw_line( Vec2::zero, marker_pos2 );
}

void Scene_Angle_Dirs::draw_ui()
{
	Scene::draw_ui();
	draw_title( "Angles / Directions" );

	scoped_render_state;

	// label next to the marker
	if( marker_pos != Vec2::zero )
	{
		Rect ui_pos( Coord_System::world_to_ui_pos( marker_pos + Vec2( 16.f, 0.f ) ), 0.f, 64.f );

		Render::state->color = make_color( e_pal::lightest );

		auto angle = Vec2::clamped_angle_from_dir( Vec2::normalize( marker_pos ) );
		auto dir = Vec2::dir_from_angle( angle );

		std::vector<std::string> info_lines =
		{
			std::format( "Angle : {:.0f} d", angle ),
			std::format( "Dir   : {:.1f}, {:.1f}", dir.x, dir.y )
		};

		if( !marker_pos2.is_zero() )
		{
			info_lines.push_back( std::format( "Dot   : {:.3f}", Vec2::dot( marker_pos, marker_pos2 ) ) );
		}

		Render::draw_string( info_lines, ui_pos );
	}
}

bool Scene_Angle_Dirs::on_input_motion( const Input_Event* evt )
{
	switch( evt->input_id )
	{
		case e_input_id::mouse:
		{
			if( g_engine->input_mgr.is_button_held( e_input_id::mouse_button_left ) )
			{
				marker_pos = Coord_System::window_to_world_pos( evt->mouse_pos );
				return true;
			}

			if( g_engine->input_mgr.is_button_held( e_input_id::mouse_button_right ) )
			{
				marker_pos2 = Coord_System::window_to_world_pos( evt->mouse_pos );
				return true;
			}
		}
		break;
	}

	return false;
}
