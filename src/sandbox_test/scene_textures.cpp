
#include "app_header.h"

using namespace war;

scene_textures::scene_textures()
{
	flags.blocks_further_drawing = true;
}

void scene_textures::pushed()
{
	scene::pushed();

	tex_crate = g_engine->find_asset<texture_asset>( "tex_crate_01" );
	grad_horizontal = g_engine->find_asset<texture_asset>( "gradient_horizontal" );
	grad_vertical = g_engine->find_asset<texture_asset>( "gradient_vertical" );
	anim_mario = g_engine->find_asset<texture_asset>( "anim_player_run" );
	anim_coin = g_engine->find_asset<texture_asset>( "anim_coin" );

	anim_offsets[ 0 ] = random::getf();
	anim_offsets[ 1 ] = random::getf();
	anim_offsets[ 2 ] = random::getf();

	viewport_pivot = vec2::zero;
	g_engine->window.set_mouse_mode( mouse_mode::os );
}

void scene_textures::draw()
{
	draw_tiled_background();
	scene::draw();
	render::draw_world_axis();
}

void scene_textures::draw_ui()
{
	scene::draw_ui();
	draw_title( "Textures" );

	{
		scoped_render_state;

		vec2 origin;

		// info boxes

		auto l_draw_info_box = [] ( std::string_view caption, const rect& rc )
		{
			render::state->color = make_color( color::black, 0.25f );
			render::draw_rounded_filled_rect( rc, 6.f );

			render::state->color = make_color( color::white, 0.75f );
			render::state->align = align::centered;
			render::draw_string( caption, vec2( rc.x + ( rc.w / 2.f ), rc.y + 9.f ) );
		};

		l_draw_info_box( "quads", { 6.f, 32.f, 84.f, 92.f } );
		l_draw_info_box( "sprites", { 96.f, 32.f, 84.f, 92.f } );
		l_draw_info_box( "gradients", { 186.f, 32.f, 116.f, 76.f } );
		l_draw_info_box( "tiled", { 10.f, 132.f, 174.f, 104.f } );
		l_draw_info_box( "animated", { 194.f, 128.f, 108.f, 108.f } );

		render::state->color = make_color( color::white );

		// crate, quad

		origin = { 16.f, 52.f };
		render::draw_quad( tex_crate, origin );
		render::draw_filled_circle( origin, 2.f );

		// crate, sprite

		origin = { 138.f, 84.f };
		render::draw_sprite( tex_crate, origin );
		render::draw_filled_circle( origin, 2.f );

		// crate, tiled

		origin = { 16.f, 152.f };
		render::draw_tiled( tex_crate, rect( origin.x, origin.y, 160.f, 76.f ) );

		// gradient, horizontal

		origin = { 192.f, 52.f };
		render::draw_quad( grad_horizontal, rect( origin.x, origin.y, 48.f, 48.f ) );

		// gradient, vertical

		origin = { 248.f, 52.f };
		render::draw_quad( grad_vertical, rect( origin.x, origin.y, 48.f, 48.f ) );

		// mario, animating

		origin = { 200.f, 148.f };
		render::state->scale = 2.f;
		render::draw_quad( anim_mario, vec2( origin.x, origin.y ) );

		// coins, showing animation offsets

		origin = { 200.f, 192.f };
		render::state->anim_offset = anim_offsets[ 0 ];
		render::draw_quad( anim_coin, vec2( origin.x, origin.y ) );

		origin.x += 32.f;
		render::state->anim_offset = anim_offsets[ 1 ];
		render::draw_quad( anim_coin, vec2( origin.x, origin.y ) );

		origin.x += 32.f;
		render::state->anim_offset = anim_offsets[ 2 ];
		render::draw_quad( anim_coin, vec2( origin.x, origin.y ) );
	}
}
