
#include "app_header.h"

using namespace war;

Scene_Menu::Scene_Menu()
{
	flags.blocks_further_drawing = true;
	flags.requires_controller = true;
	flags.is_main_menu = true;
}

void Scene_Menu::pushed()
{
	Scene::pushed();

	pulse_tween = Tween( 0.f, 1.f, 500, e_tween_type::pingpong, e_tween_via::sinusoidal );
}

void Scene_Menu::draw_ui()
{
	Scene::draw_ui();

	Render::state->align = e_align::centered;
	Render::state->color = make_color( e_pal::lightest, *pulse_tween );
	Render::draw_string( "Press any button to play...", Vec2( viewport_hw, viewport_hh ) );
}

bool Scene_Menu::on_input_pressed( const Input_Event* evt )
{
	g_engine->scene_mgr.push<Scene_Gameplay>();
	return true;
}

