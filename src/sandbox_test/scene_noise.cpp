
#include "app_header.h"

using namespace war;

Scene_Noise::Scene_Noise()
{
	flags.blocks_further_drawing = true;
	flags.blocks_further_update = true;

	memset( &perlin_samples[ 0 ], 0, sizeof( f32 ) * 64 );
	memset( &simplex_samples[ 0 ], 0, sizeof( f32 ) * 64 );
	memset( &random_samples[ 0 ], 0, sizeof( f32 ) * 64 );
}

void Scene_Noise::pushed()
{
	Scene::pushed();

	viewport_pivot = Vec2::zero;

	generate_noise_samples();
}

void Scene_Noise::draw()
{
	draw_tiled_background();
	Scene::draw();
}

void Scene_Noise::draw_ui()
{
	Scene::draw_ui();
	draw_title( "Noise" );

	auto cb = get_ui_callback();

	{
		scoped_render_state;

		constexpr f32 noise_sz = 16.f;

		auto l_draw_noise = [&] ( f32 y_pos, f32 samples [] )
		{
			y_pos += g_engine->pixel_font->get_max_height();

			Render::state->color = make_color( e_pal::darker, 0.5f );
			Render::draw_filled_rect( Rect( 0.f, y_pos, ui_w, noise_sz * 2.5f ) );

			Render::state->color = make_color( e_pal::darkest, 0.5f );
			Render::draw_filled_rect( Rect( 0.f, y_pos + ( noise_sz * 1.25f ), ui_w, 1.f ) );

			Render::state->color = make_color( e_pal::brightest, 1.f );

			Vec2 start = { 0.f, y_pos + ( noise_sz * 1.25f ) };
			Vec2 end;

			constexpr int steps = 64;
			f32 pixels_per_step = ui_w / (f32)steps;

			for( int x = 0 ; x < steps ; ++x )
			{
				start.x = x * pixels_per_step;
				end.x = start.x + pixels_per_step;

				end.y = y_pos + ( noise_sz * 1.25f ) + ( samples[ x ] * noise_sz );

				Render::state->color = make_color( e_pal::brighter, 0.5f );
				Render::state->glow = 0.f;
				Render::draw_line( start, end );


				Render::state->color = make_color( e_pal::brightest, 1.f );
				Render::state->glow = 6.f;
				Render::draw_point( start );

				start = end;
			}

			Render::state->glow = 0.f;
		};

		Render::state->color = make_color( e_pal::brightest );

		Render::draw_string( "Simplex", { 4.f, 64.f } );
		l_draw_noise( 64.f, simplex_samples );

		Render::draw_string( "Perlin", { 4.f, 112.f } );
		l_draw_noise( 112.f, perlin_samples );

		Render::draw_string( "Random", { 4.f, 166.f } );
		l_draw_noise( 166.f, random_samples );
	}

	Rect rc_panel = { 0.f, 0.f, ui_w, ui_h };
	rc_panel.shrink( 32.f );
	g_ui->layout_init( rc_panel );

	{
		scoped_render_state;

		if( g_ui->button_control()
			->set_text( "Refresh" )
			->done() )
		{
			generate_noise_samples();
		}
	}
}

void Scene_Noise::generate_noise_samples()
{
	for( auto x = 0 ; x < 64 ; ++x )
	{
		simplex_samples[ x ] = simplex.get();
		perlin_samples[ x ] = perlin.get();
		random_samples[ x ] = random.get();
	}
}
