
#include "master_pch.h"
#include "master_header.h"

namespace war
{

Scene_Transient::Scene_Transient()
{
	flags.blocks_further_input = true;
	flags.blocks_further_update = true;
}

/*
void scene_transient::draw_ui()
{
	scene::draw_ui();

	render::draw_string( "TRANSIENT", vec2( 8.f, 8.f ) );
}
*/

}
