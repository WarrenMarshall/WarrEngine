
#include "app_header.h"

using namespace war;

Scene_Textures::Scene_Textures()
{
	flags.blocks_further_drawing = true;
}

void Scene_Textures::pushed()
{
	Scene::pushed();

	tex_crate = g_engine->find_asset<Texture_Asset>( "tex_crate_01" );
	grad_horizontal = g_engine->find_asset<Texture_Asset>( "gradient_horizontal" );
	grad_vertical = g_engine->find_asset<Texture_Asset>( "gradient_vertical" );
	anim_player = g_engine->find_asset<Texture_Asset>( "anim_player_run" );
	anim_coin = g_engine->find_asset<Texture_Asset>( "anim_coin" );

	anim_offsets[ 0 ] = Random::getf();
	anim_offsets[ 1 ] = Random::getf();
	anim_offsets[ 2 ] = Random::getf();

	viewport_pivot = Vec2::zero;
	g_engine->window.set_mouse_mode( e_mouse_mode::os );
}

void Scene_Textures::draw()
{
	draw_tiled_background();
	Scene::draw();
	Render::draw_world_axis();
}

void Scene_Textures::draw_ui()
{
	Scene::draw_ui();
	draw_title( "Textures" );

	{
		scoped_render_state;

		Vec2 origin;

		// info boxes

		auto l_draw_info_box = [] ( const std::string& caption, const Rect& rc )
		{
			Render::state->color = make_color( Color::black, 0.25f );
			Render::draw_rounded_filled_rect( rc, 6.f );

			Render::state->color = make_color( Color::white, 0.75f );
			Render::state->align = e_align::centered;
			Render::draw_string( caption, Vec2( rc.x + ( rc.w / 2.f ), rc.y + 9.f ) );
		};

		l_draw_info_box( "quads", { 6.f, 32.f, 84.f, 92.f } );
		l_draw_info_box( "sprites", { 96.f, 32.f, 84.f, 92.f } );
		l_draw_info_box( "gradients", { 186.f, 32.f, 116.f, 76.f } );
		l_draw_info_box( "tiled", { 10.f, 132.f, 174.f, 104.f } );
		l_draw_info_box( "animated", { 194.f, 128.f, 108.f, 108.f } );

		Render::state->color = make_color( Color::white );

		// crate, quad

		origin = { 16.f, 52.f };
		Render::draw_quad( tex_crate, origin );
		Render::draw_filled_circle( origin, 2.f );

		// crate, sprite

		origin = { 138.f, 84.f };
		Render::draw_sprite( tex_crate, origin );
		Render::draw_filled_circle( origin, 2.f );

		// crate, tiled

		origin = { 16.f, 152.f };
		Render::draw_tiled( tex_crate, Rect( origin.x, origin.y, 160.f, 76.f ) );

		// gradient, horizontal

		origin = { 192.f, 52.f };
		Render::draw_quad( grad_horizontal, Rect( origin.x, origin.y, 48.f, 48.f ) );

		// gradient, vertical

		origin = { 248.f, 52.f };
		Render::draw_quad( grad_vertical, Rect( origin.x, origin.y, 48.f, 48.f ) );

		// player, animating

		origin = { 200.f, 148.f };
		Render::state->scale = 2.f;
		Render::draw_quad( anim_player, Vec2( origin.x, origin.y ) );

		// coins, showing animation offsets

		origin = { 200.f, 192.f };
		Render::state->anim_offset = anim_offsets[ 0 ];
		Render::draw_quad( anim_coin, Vec2( origin.x, origin.y ) );

		origin.x += 32.f;
		Render::state->anim_offset = anim_offsets[ 1 ];
		Render::draw_quad( anim_coin, Vec2( origin.x, origin.y ) );

		origin.x += 32.f;
		Render::state->anim_offset = anim_offsets[ 2 ];
		Render::draw_quad( anim_coin, Vec2( origin.x, origin.y ) );
	}
}
