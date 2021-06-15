
#include "app_header.h"

using namespace war;

scene_noise::scene_noise()
{
	flags.blocks_further_drawing = true;

	memset( &perlin_samples[ 0 ], 0, sizeof( float ) * 64 );
	memset( &simplex_samples[ 0 ], 0, sizeof( float ) * 64 );
	memset( &random_samples[ 0 ], 0, sizeof( float ) * 64 );
}

void scene_noise::pushed()
{
	viewport_pivot = vec2::zero;
	g_engine->window.set_mouse_mode( mouse_mode::custom );

	generate_noise_samples();
}

void scene_noise::draw()
{
	draw_tiled_background();
	scene::draw();
}

void scene_noise::draw_ui()
{
	scene::draw_ui();
	draw_title( "Noise" );

	auto cb = get_ui_callback();

	{
		scoped_render_state;

		constexpr float noise_sz = 16.f;

		auto l_draw_noise = [ & ]( float y_pos, float samples [] )
		{
			y_pos += g_engine->pixel_font->get_max_height();

			render::state->color = make_color( 1, 0.5f );
			render::draw_filled_rect( rect( 0.f, y_pos, ui_w, noise_sz * 2.5f ) );

			render::state->color = make_color( 0, 0.5f );
			render::draw_filled_rect( rect( 0.f, y_pos + ( noise_sz * 1.25f ), ui_w, 1.f ) );

			render::state->color = make_color( 4, 1.f );

			vec2 start = { 0.f, y_pos + ( noise_sz * 1.25f ) };
			vec2 end;

			constexpr int steps = 64;
			float pixels_per_step = ui_w / (float)steps;

			for( int x = 0 ; x < steps ; ++x )
			{
				start.x = x * pixels_per_step;
				end.x = start.x + pixels_per_step;

				end.y = y_pos + ( noise_sz * 1.25f ) + ( samples[ x ] * noise_sz );

				render::state->color = make_color( pal::brighter, 0.5f );
				render::state->glow = 0.f;
				render::draw_line( start, end );


				render::state->color = make_color( pal::brightest, 1.f );
				render::state->glow = 6.f;
				render::draw_point( start );

				start = end;
			}

			render::state->glow = 0.f;
		};

		render::state->color = make_color( pal::brightest );

		render::draw_string( "Simplex", { 4.f, 64.f } );
		l_draw_noise( 64.f, simplex_samples );

		render::draw_string( "Perlin", { 4.f, 112.f } );
		l_draw_noise( 112.f, perlin_samples );

		render::draw_string( "Random", { 4.f, 166.f } );
		l_draw_noise( 166.f, random_samples );
	}

	rect rc_panel = { 0.f, 0.f, ui_w, ui_h };
	rc_panel.shrink( 32.f );
	g_ui->layout_init( rc_panel );

	{
		scoped_render_state;

		if( g_ui->button_control()
			->set_text( "Refresh" )
			->end() )
		{
			generate_noise_samples();
		}
	}
}

void scene_noise::generate_noise_samples()
{
	perlin.init();
	simplex.init();
	random.init();

	for( auto x = 0 ; x < 64 ; ++x )
	{
		simplex_samples[ x ] = simplex.get();
		perlin_samples[ x ] = perlin.get();
		random_samples[ x ] = random.get();
	}
}
