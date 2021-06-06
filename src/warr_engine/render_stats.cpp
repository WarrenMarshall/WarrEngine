
#include "master_pch.h"
#include "master_header.h"

namespace war
{

void render_stats::init()
{
	stat_timer = timer( 1000 );
}

void render_stats::update()
{
	if( stat_timer.is_elapsed() )
	{
		stat_timer.restart();

		frame_count.update_value();
		auto steps = (int)( frame_count.value );

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

void render_stats::draw()
{
#if !defined(_FINAL_RELEASE)
	{
		scoped_render_state;

		stat_strings.clear();
		render::state->z = zdepth_stats;

		if( draw_verbose )
		{
			stat_strings.clear();
			stat_strings.emplace_back( std::format( "RENDER : {} FPS ({:.1f} ms) / FTS: {} FPS",
				f_commas( frame_count.value ),
				frame_times_ms.value,
				fixed_time_step::frames_per_second ) );
			stat_strings.emplace_back( std::format( "DC:{} / Q:{} / T:{} / L:{} / P:{}",
				f_commas( draw_calls.value ),
				f_commas( quads.value ),
				f_commas( triangles.value ),
				f_commas( lines.value ),
				f_commas( points.value ) )
			);
			stat_strings.emplace_back( std::format( "Scenes : {}, Entities : {}", g_engine->scenes.scene_stack.size(), f_commas( entities.value ) ) );
			stat_strings.emplace_back( std::format( "Time Dilation: {:.2f}", g_engine->time.dilation ) );

			auto window_pos = g_engine->input.mouse_window_pos;
			auto viewport_pos = coord_system::window_to_viewport_pos( window_pos );
			auto world_pos = coord_system::viewport_to_world_pos( viewport_pos );
			auto ui_pos = coord_system::viewport_to_ui_pos( viewport_pos );

			stat_strings.emplace_back( std::format( "wn:{:.0f}/{:.0f} | wd:{:.0f}/{:.0f} | vp:{:.0f}/{:.0f} | ui:{:.0f}/{:.0f}",
				window_pos.x, window_pos.y,
				world_pos.x, world_pos.y,
				viewport_pos.x, viewport_pos.y,
				ui_pos.x, ui_pos.y )
			);

			auto cam_transform = g_engine->scenes.get_transform();
			stat_strings.emplace_back( std::format( "cam: {:.0f}/{:.0f} | {:.1f} d | {:.2f} s",
				cam_transform->pos.x, cam_transform->pos.y, cam_transform->angle, cam_transform->scale )
			);

			if( stat_custom_string.length() )
			{
				stat_strings.emplace_back( stat_custom_string );
				stat_custom_string.clear();
			}

			{
				scoped_render_state;

				render::state->color = make_color( 0, 0.5f );
				render::draw_filled_rect(
					rect( 0.f, 0.f,
					ui_w, (float)( g_engine->pixel_font->get_max_height() * stat_strings.size() ) )
				);

				render::state->z += zdepth_nudge;
				render::state->align = align::hcenter;
				render::state->color = color::white;

				auto ypos = 0.f;
				for( const auto& iter : stat_strings )
				{
					render::draw_string( iter, { ui_hw, ypos } );
					ypos += g_engine->pixel_font->get_max_height();
				}
			}
		}
		else
		{
			if( frame_count.value > 1.f )
			{
				scoped_render_state;

				render::state->align = align::right;
				render::draw_string(
					std::format( "{} FPS ({:.2f} ms)", f_commas( frame_count.value ), frame_times_ms.value ),
					{ ui_w, 0.f } );
			}
		}
	}
#endif
}

}
