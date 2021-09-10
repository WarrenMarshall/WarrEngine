
#include "master_pch.h"
#include "master_header.h"

namespace war
{

Vec2 Scene_Mgr::get_viewport_pivot()
{
	return current_scene->get_viewport_pivot();
}

void Scene_Mgr::clear_stack()
{
	// give each scene a chance to clean up before the manager shuts down.

	for( const auto& scene : scene_stack )
	{
		scene->getting_covered();
		scene->popped();
		scene->life_cycle.set( e_life_cycle::dead );
	}
}

void Scene_Mgr::pop()
{
	assert( !scene_stack.empty() );

	Scene* scene = nullptr;
	size_t idx;

	for( idx = 0 ; idx < scene_stack.size() ; ++idx )
	{
		scene = scene_stack[ idx ].get();

		if( scene->life_cycle.is_alive() )
		{
			scene->popped();
			scene->life_cycle.set( e_life_cycle::dead );
			break;
		}
	}

	// since the top scene is being popped, we can tell the next scene in the stack
	// that it is being shown.
	if( scene_stack.size() > 1 )
	{
		scene = scene_stack[ idx + 1 ].get();
		scene->becoming_top_scene();
	}
}

void Scene_Mgr::pop_under()
{
	assert( !scene_stack.empty() );

	Scene* scene = nullptr;
	size_t idx = 1;
	scene = scene_stack[ idx ].get();
	scene->popped();
	scene->life_cycle.set( e_life_cycle::dead );
}

Scene* Scene_Mgr::get_top()
{
	assert( !scene_stack.empty() );

	return scene_stack.front().get();
}

// ----------------------------------------------------------------------------

void Scene_Mgr::pre_update()
{
	// remove dead scenes

	for( size_t x = 0 ; x < scene_stack.size() ; ++x )
	{
		if( scene_stack[ x ]->life_cycle.is_dead() )
		{
			scene_stack.erase( scene_stack.begin() + x );
			x--;
		}
	}

	// update living scenes

	for( auto& iter : scene_stack )
	{
		iter->pre_update();

		if( iter->flags.blocks_further_update )
		{
			return;
		}
	}
}

void Scene_Mgr::update()
{
	if( transition_timeline.life_cycle.is_alive() )
	{
		transition_timeline.update();

		if( !transition_timeline.life_cycle.is_alive() )
		{
			pop();
		}
	}

	for( auto& iter : scene_stack )
	{
		iter->update();

		if( iter->flags.blocks_further_update )
		{
			return;
		}
	}

	// if the game requires a game controller and one is not detected, put up
	// the dialog that waits for one to get connected.

	if( g_engine->scene_mgr.current_scene->flags.requires_controller and !g_engine->input_mgr.gamepad )
	{
		g_engine->scene_mgr.push<Scene_Controller_Required>();
	}
}

void Scene_Mgr::post_update()
{
	for( auto& iter : scene_stack )
	{
		iter->post_update();

		if( iter->flags.blocks_further_update )
		{
			return;
		}
	}
}

// ----------------------------------------------------------------------------

void Scene_Mgr::draw()
{
	if( !scene_stack.empty() )
	{
		// Locate the first completely opaque scene, starting at the top
		// and moving backward.

		int32_t starting_scene_idx = find_first_fully_opaque_scene();

		// draw starting from the starting_scene_idx and iterate through every
		// scene above it

		draw_scene( starting_scene_idx );

		// does not respond to scene transforms. UI is always drawn with the
		// origin in the top/left corner of the viewport and doesn't move,
		// rotate, shake, etc.

		draw_scene_ui( starting_scene_idx );
	}
}

// iterates through the scene list, front to back, looking for the first scene
// that is drawing opaquely. drawing will start from there and walk back up the
// stack to the top.

int32_t Scene_Mgr::find_first_fully_opaque_scene()
{
	int32_t idx = -1;

	for( const auto& iter : scene_stack )
	{
		idx++;

		if( iter->flags.blocks_further_drawing )
		{
			break;
		}
	}

	// if we never find an opaque scene something has been set up
	// wrong. make sure that at least 1 scene in the stack is marked as
	// "draws_completely_solid = true"

	assert( idx > -1 );

	return idx;
}

void Scene_Mgr::draw_scene( int32_t starting_scene_idx )
{
	scoped_render_state;
	auto scene_depth_start = 0.f;

	for( auto x = starting_scene_idx; x >= 0; --x )
	{
		auto scene = scene_stack[ x ].get();

		if( scene->life_cycle.is_alive() )
		{
			current_scene = scene;

			g_engine->opengl_mgr.clear_depth_buffer();
			scene_depth_start += zdepth_scene_start;
			Render::state->z = scene_depth_start;

			g_engine->opengl_mgr.set_view_matrix_identity();

			{
				scoped_render_state;
				scene->draw();


				#ifndef _FINAL_RELEASE
					// draw any debug information that lives in world space.
					if( scene->flags.is_debug_physics_scene && g_engine->render.debug.is_drawing_debug_info() )
					{
						Render::state->z += zdepth_debug_bias;
						g_engine->box2d.world->DebugDraw();
					}
				#endif
			}

			current_scene = get_top();

			g_engine->render.dynamic_batches.flush_and_reset( e_draw_call::opaque );
			g_engine->render.dynamic_batches.flush_and_reset( e_draw_call::transparent );
		}
	}
}

void Scene_Mgr::draw_scene_ui( int32_t starting_scene_idx )
{
	g_engine->opengl_mgr.set_view_matrix_identity_ui();

	scoped_render_state;
	auto scene_depth_start = 0.f;

	for( auto x = starting_scene_idx; x >= 0; --x )
	{
		auto scene = scene_stack[ x ].get();

		if( scene->life_cycle.is_alive() )
		{
			current_scene = scene;

			g_engine->opengl_mgr.clear_depth_buffer();
			scene_depth_start += zdepth_scene_start;
			Render::state->z = scene_depth_start;

			{
				scoped_opengl;

				g_ui->current_callback = scene->get_ui_callback();

				{
					scoped_render_state;
					scene->draw_ui();
				}

				scene->flags.clear_expanded_tag_this_frame = false;

				current_scene = get_top();
				g_ui->current_callback = nullptr;

				// when the final scene has been drawn, add anything on top that
				// we need to - like a mouse cursor - to contain it within the same draw call.
				if( !x )
				{
					g_ui->draw_topmost();
				}

			#ifndef _FINAL_RELEASE

				// draw a circle so we can visualize the viewport offset location
				if( g_engine->render.debug.is_drawing_debug_info() )
				{
					Render::draw_crosshair( scene->viewport_pivot / ui_scale );
				}
			#endif
			}

			g_engine->render.dynamic_batches.flush_and_reset( e_draw_call::opaque );
			g_engine->render.dynamic_batches.flush_and_reset( e_draw_call::transparent );
		}
	}
}

void Scene_Mgr::new_game()
{
	for( auto& scene : scene_stack )
	{
		if( scene->life_cycle.is_alive() )
		{
			scene->new_game();
		}
	}
}

bool Scene_Mgr::on_input_motion( const Input_Event* evt )
{
	for( const auto& iter : scene_stack )
	{
		if( iter->on_input_motion( evt ) )
		{
			return true;
		}

		if( iter->flags.blocks_further_input )
		{
			return true;
		}
	}

	return false;
}

bool Scene_Mgr::on_input_pressed( const Input_Event* evt )
{
	for( const auto& iter : scene_stack )
	{
		if( iter->on_input_pressed( evt ) )
		{
			return true;
		}

		if( iter->flags.blocks_further_input )
		{
			return true;
		}
	}

	return false;
}

bool Scene_Mgr::on_input_held( const Input_Event* evt )
{
	for( const auto& iter : scene_stack )
	{
		if( iter->on_input_held( evt ) )
		{
			return true;
		}

		if( iter->flags.blocks_further_input )
		{
			return true;
		}
	}

	return false;
}

bool Scene_Mgr::on_input_released( const Input_Event* evt )
{
	for( const auto& iter : scene_stack )
	{
		if( iter->on_input_released( evt ) )
		{
			return true;
		}

		if( iter->flags.blocks_further_input )
		{
			return true;
		}
	}

	return false;
}

bool Scene_Mgr::on_input_key( const Input_Event* evt )
{
	for( const auto& iter : scene_stack )
	{
		if( iter->on_input_key( evt ) )
		{
			return true;
		}

		if( iter->flags.blocks_further_input )
		{
			return true;
		}
	}

	return false;
}

}
