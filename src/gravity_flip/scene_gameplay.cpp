
#include "app_header.h"

using namespace war;

scene_gameplay::scene_gameplay()
{
	flags.is_debug_physics_scene = true;
}

void scene_gameplay::draw_ui()
{
	scene::draw_ui();

}

void scene_gameplay::pushed()
{
	viewport_pivot = { viewport_hw, viewport_hh };

	g_engine->window.set_mouse_mode( mouse_mode::os );

	player = add_entity<entity>();
}

void scene_gameplay::update()
{
	scene::update();

}

bool scene_gameplay::on_input_pressed( const input_event* evt )
{
	if( evt->input_id == input_id::gamepad_button_a )
	{
		//player->fire();
		return true;
	}

	return false;
}
