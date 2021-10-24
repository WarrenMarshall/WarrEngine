
#include "app_header.h"

using namespace war;

Scene_Gameplay::Scene_Gameplay()
{
	flags.blocks_further_drawing = true;
	flags.blocks_further_input = true;
}

void Scene_Gameplay::pushed()
{
	Scene::pushed();

	player = add_entity<E_Player>();

}

void Scene_Gameplay::draw()
{
	Scene::draw();

}

bool Scene_Gameplay::on_input_pressed( const Input_Event* evt )
{
	if( Scene::on_input_pressed( evt ) )
	{
		return true;
	}

	return false;
}

