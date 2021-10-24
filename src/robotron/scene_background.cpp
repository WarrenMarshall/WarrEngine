
#include "app_header.h"

using namespace war;

Scene_Background::Scene_Background()
{
	flags.blocks_further_drawing = true;
}

void Scene_Background::pushed()
{
	Scene::pushed();
}

void Scene_Background::draw()
{
	Scene::draw();

}
