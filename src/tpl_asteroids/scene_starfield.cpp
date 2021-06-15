
#include "app_header.h"

using namespace war;

scene_starfield::scene_starfield()
{
	flags.blocks_further_drawing = true;
}

void scene_starfield::pushed()
{
	viewport_pivot = { viewport_hw, viewport_hh };

	gradient = g_engine->find_asset<texture_asset>( "background_gradient" );

	{
		auto e = add_entity<entity>();

		{
			auto ec = e->add_component<emitter_component>();
			ec->init( "em_starfield" );
		}
	}

	g_engine->window.set_mouse_mode( mouse_mode::os );
}

void scene_starfield::draw()
{
	render::draw_quad( gradient, rect( -viewport_hw, -viewport_hh, viewport_w, viewport_h ) );
	scene::draw();
	//w_render::draw_world_axis();
}
