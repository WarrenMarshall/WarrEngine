
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

	gradient = g_engine->find_asset<Texture_Asset>( "background_gradient" );
	tex_robotron = g_engine->find_asset<Texture_Asset>( "tex_hello_world" );
	movement_tween = Tween( -175, 175, 4000, e_tween_type::pingpong, e_tween_via::quadratic );
	scale_tween = Tween( .5f, 5.f, 9000, e_tween_type::pingpong, e_tween_via::sinusoidal );
	tilt_tween = Tween( -25.f, 25.f, 20000, e_tween_type::pingpong, e_tween_via::cubic );

}

void Scene_Gameplay::draw()
{
	Scene::draw();


	Render::draw_quad( gradient, Rect( -viewport_hw, -viewport_hh, viewport_w, viewport_h ) );
	Render::state->z += zdepth_nudge;

	Render::state->angle = *tilt_tween;
	Render::state->scale = *scale_tween;
	Render::draw_sprite( tex_robotron, Vec2( *movement_tween, 0.f ) );
}

bool Scene_Gameplay::on_input_pressed( const Input_Event* evt )
{
	if( Scene::on_input_pressed( evt ) )
	{
		return true;
	}

	return false;
}

