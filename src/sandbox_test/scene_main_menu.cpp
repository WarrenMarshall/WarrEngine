
#include "app_header.h"

using namespace war;

scene_main_menu::scene_main_menu()
{
	flags.blocks_further_drawing = true;
}

void scene_main_menu::pushed()
{
	scene::pushed();

	viewport_pivot = Vec2::zero;
	g_engine->window.set_mouse_mode( mouse_mode::os );

}

void scene_main_menu::draw()
{
	scene::draw();
	draw_gradient_background();
	Render::draw_world_axis();
}

void scene_main_menu::draw_ui()
{
	scene::draw_ui();

	auto border_sz = 4.f;
	auto button_h = 12.f;

	auto rc_panel = Rect( border_sz, border_sz, ui_w - ( border_sz * 2.f ), ui_h - ( border_sz * 2.f ) );
	g_ui->layout_init( rc_panel );

	g_ui->panel_control( H( "main_panel" ) )
		->set_rect( rc_panel )
		->center_control_on_screen()
		->adjust_layout_to_client_area()
		->done();

	{
		g_ui->caption_control()
			->set_text( "Test Scenes Available" )
			->done();

		{
			scoped_ui_layout( g_ui->layout_top().cut_left( 0.5f ) );

			do_menu_button<scene_palette>( H( "button_palettes" ), "Palettes" );

			g_ui->divider_control()->done();
			do_menu_button<scene_angle_dirs>( H( "angle_dirs" ), "Angles / Directions" );
			do_menu_button<scene_coords>( H( "button_coords" ), "Coords" );

			g_ui->divider_control()->done();
			do_menu_button<scene_ui>( H( "button_ui" ), "UI Controls" );
			do_menu_button<scene_wrapped_text>( H( "button_wrapped_text" ), "Wrapped Text" );

			g_ui->divider_control()->done();
			do_menu_button<scene_textures>( H( "button_textures" ), "Textures" );
			do_menu_button<scene_meshes>( H( "button_meshes" ), "Meshes" );
			do_menu_button<scene_particles>( H( "button_particles" ), "Particles" );
			do_menu_button<scene_tilemap>( H( "button_tilemap" ), "Tile Map" );
		}

		{
			scoped_ui_layout( g_ui->layout_top().cut_right( 0.5f ) );

			do_menu_button<scene_entity_picking>( H( "button_entity_picking" ), "Entity Picking" );
			do_menu_button<scene_tweens>( H( "button_tweens" ), "Tweens / Timers" );
			do_menu_button<scene_sound_board>( H( "button_sound_board" ), "Sound Board" );

			g_ui->divider_control()->done();
			do_menu_button<scene_box2d>( H( "button_box2d" ), "Box2D Collision" );
			do_menu_button<scene_simple_collision>( H( "button_simple_collision" ), "Simple Collision" );
			do_menu_button<scene_simple_interact>( H( "button_simple_interact" ), "Simple Interaction" );
			do_menu_button<scene_simple_bounce>( H( "button_simple_space" ), "Simple Bouncing" );

			g_ui->divider_control()->done();
			do_menu_button<scene_simple_breakout>( H( "button_simple_breakout" ), "Break Out" );
			do_menu_button<scene_simple_platformer>( H( "button_simple_platformer" ), "Platformer" );
			do_menu_button<scene_simple_top_down>( H( "button_simple_top_down" ), "Top Down" );

			g_ui->divider_control()->done();
			do_menu_button<scene_controller>( H( "button_controller" ), "Game Controller" );
			do_menu_button<scene_noise>( H( "button_noise" ), "Noise" );
		}
	}
}
