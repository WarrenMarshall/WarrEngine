
#include "master_pch.h"
#include "master_header.h"

namespace war
{

scene_timeline::scene_timeline()
{
	flags.blocks_further_input = true;
	flags.blocks_further_update = true;
}

void scene_timeline::pushed()
{
	scene::pushed();

	fx_timeline.go();
}

void scene_timeline::update()
{
	scene::update();

	fx_timeline.update();

	if( !fx_timeline.life_cycle.is_alive() )
	{
		g_engine->scenes.pop();
	}
}

}
