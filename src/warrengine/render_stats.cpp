
#include "master_pch.h"
#include "master_header.h"

namespace war
{

#if !defined(_FINAL_RELEASE)

void Render_Stats::init()
{
	stat_timer = Timer( 1000 );
}

void Render_Stats::update()
{

	if( stat_timer.is_elapsed() )
	{
		stat_timer.restart();

		frame_count.update_value();
		auto steps = ( int32_t )( frame_count.value );

		frame_times_ms.update_value( steps );
		draw_calls.update_value( steps );
		entities.update_value( steps );
		quads.update_value( steps );
		triangles.update_value( steps );
		lines.update_value( steps );
		points.update_value( steps );
	}
}

// draws useful stats at the top of the screen

void Render_Stats::draw()
{
	{
		scoped_render_state;

		stat_strings.clear();
		Render::state->z = zdepth_stats;

		if( draw_verbose )
		{
			stat_strings.clear();
			stat_strings.push_back( std::format( "RENDER : {} FPS ({:.1f} ms) / FTS: {} FPS",
				f_commas( frame_count.value ),
				frame_times_ms.value,
				fixed_time_step::frames_per_second ) );
			stat_strings.push_back( std::format( "DC:{} / Q:{} / T:{} / L:{} / P:{}",
				f_commas( draw_calls.value ),
				f_commas( quads.value ),
				f_commas( triangles.value ),
				f_commas( lines.value ),
				f_commas( points.value ) )
			);
			stat_strings.push_back( std::format( "Scenes : {}, Entities : {}", g_engine->scene_mgr.scene_stack.size(), f_commas( entities.value ) ) );
			stat_strings.push_back( std::format( "Time Dilation: {:.2f}", g_engine->clock.dilation ) );

			auto window_pos = g_engine->input_mgr.mouse_window_pos;
			auto viewport_pos = Coord_System::window_to_viewport_pos( window_pos );
			auto world_pos = Coord_System::viewport_to_world_pos( viewport_pos );
			auto ui_pos = Coord_System::viewport_to_ui_pos( viewport_pos );

			stat_strings.push_back( std::format( "wn:{:.0f}/{:.0f} | wd:{:.0f}/{:.0f} | vp:{:.0f}/{:.0f} | ui:{:.0f}/{:.0f}",
				window_pos.x, window_pos.y,
				world_pos.x, world_pos.y,
				viewport_pos.x, viewport_pos.y,
				ui_pos.x, ui_pos.y )
			);

			auto cam_transform = g_engine->scene_mgr.get_transform();
			stat_strings.push_back( std::format( "cam: {:.0f}/{:.0f} | {:.1f} d | {:.2f} s",
				cam_transform->pos.x, cam_transform->pos.y, cam_transform->angle, cam_transform->scale )
			);

			stat_strings.push_back( std::format( "qt: {} nodes",
				g_engine->scene_mgr.current_scene->spatial_map.nodes.count )
			);

			if( stat_custom_string.length() )
			{
				stat_strings.push_back( stat_custom_string );
				stat_custom_string.clear();
			}

			{
				scoped_render_state;

				Render::state->color = make_color( e_pal::darkest, 0.5f );
				Render::draw_filled_rect(
					Rect( 0.f, 0.f,
						ui_w, ( float_t )( g_engine->pixel_font->get_max_height() * stat_strings.size() ) )
				);

				Render::state->z += zdepth_nudge;
				Render::state->align = e_align::hcenter;
				Render::state->color = Color::white;

				auto ypos = 0.f;
				for( const auto& iter : stat_strings )
				{
					Render::draw_string( iter, { ui_hw, ypos } );
					ypos += g_engine->pixel_font->get_max_height();
				}
			}
		}
		else
		{
			if( frame_count.value > 1.f )
			{
				scoped_render_state;

				Render::state->align = e_align::right;
				Render::draw_string(
					std::format( "{} FPS ({:.2f} ms)", f_commas( frame_count.value ), frame_times_ms.value ),
					{ ui_w, 0.f } );
			}
		}
	}
}

#else

void Render_Stats::init()
{
}

void Render_Stats::update()
{
}

void Render_Stats::draw()
{
}

#endif

}
