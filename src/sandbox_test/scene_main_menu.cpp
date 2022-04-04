
#include "app_header.h"

using namespace war;

Scene_Main_Menu::Scene_Main_Menu()
{
	flags.blocks_further_drawing = true;
	flags.blocks_further_update = true;
	flags.is_main_menu = true;
}

void Scene_Main_Menu::pushed()
{
	Scene::pushed();

	viewport_pivot = Vec2::zero;
}

void Scene_Main_Menu::draw()
{
	Scene::draw();
	Render::draw_world_axis();

	// DEBUG
	//Render::draw_string( std::format( "SHIFT : {}", g_engine->input_mgr.is_shift_down() ), Vec2::zero );
}

void Scene_Main_Menu::draw_ui()
{
	Scene::draw_ui();

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

			do_menu_button<Scene_Palette>( H( "button_palettes" ), "Palettes" );

			g_ui->divider_control()->done();
			do_menu_button<Scene_Angle_Dirs>( H( "angle_dirs" ), "Angles / Directions" );
			do_menu_button<Scene_Coords>( H( "button_coords" ), "Coords" );

			g_ui->divider_control()->done();
			do_menu_button<Scene_UI>( H( "button_ui" ), "UI Controls" );
			do_menu_button<Scene_Wrapped_Text>( H( "button_wrapped_text" ), "Wrapped Text" );

			g_ui->divider_control()->done();
			do_menu_button<Scene_Textures>( H( "button_textures" ), "Textures" );
			do_menu_button<Scene_Meshes>( H( "button_meshes" ), "Meshes" );
			do_menu_button<Scene_Particles>( H( "button_particles" ), "Particles" );
			do_menu_button<Scene_Tile_Map>( H( "button_tilemap" ), "Tile Map" );

			g_ui->divider_control()->done();
			do_menu_button<Scene_Entity_Picking>( H( "button_entity_picking" ), "Entity Picking" );
			do_menu_button<Scene_Tweens_Timers>( H( "button_tweens" ), "Tweens / Timers" );
			do_menu_button<Scene_Sound_Music>( H( "button_sound_board" ), "Sound Board" );

			g_ui->divider_control()->done();
			do_menu_button<Scene_Controller>( H( "button_controller" ), "Game Controller" );
			do_menu_button<Scene_Noise>( H( "button_noise" ), "Noise" );
		}

		{
			scoped_ui_layout( g_ui->layout_top().cut_right( 0.5f ) );

			do_menu_button<Scene_Box2D>( H( "button_box2d" ), "Box2D Collision" );
			do_menu_button<Scene_Sensors>( H( "button_sensors" ), "Collision: Sensors" );
			do_menu_button<Scene_Interact>( H( "button_interact" ), "Collision: Interaction" );
			do_menu_button<Scene_Bounce>( H( "button_bounce" ), "Collision: Bouncing" );

			g_ui->divider_control()->done();
			do_menu_button<Scene_Breakout>( H( "button_breakout" ), "Game: Break Out" );
			do_menu_button<Scene_Platformer>( H( "button_platformer" ), "Game: Platformer" );
			do_menu_button<Scene_Top_Down>( H( "button_top_down" ), "Game: Top Down" );

			g_ui->divider_control()->done();
			//do_menu_button<Scene_Constraints>( H( "button_constraints" ), "Constraints" );
			do_menu_button<Scene_Spatial>( H( "button_spatial" ), "Spatial" );
			//do_menu_button<Scene_Cameras>( H( "button_cameras" ), "Cameras" );
		}
	}
}
