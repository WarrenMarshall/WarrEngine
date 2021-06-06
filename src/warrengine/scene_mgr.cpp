
#include "master_pch.h"
#include "master_header.h"

namespace war
{

transform* scene_mgr::get_transform()
{
	return current_scene->get_transform();
}

vec2 scene_mgr::get_viewport_pivot()
{
	return current_scene->get_viewport_pivot();
}

void scene_mgr::clear_stack()
{
	// give each scene a chance to clean up before the manager shuts down.

	for( const auto& scene : scene_stack )
	{
		scene->getting_covered();
		scene->popped();
		scene->life_cycle.set( life_cycle::dead );
	}
}

void scene_mgr::pop()
{
	if( scene_stack.empty() )
	{
		log_warning( "No scenes on stack - pop failed!" );
	}

	scene* scene = nullptr;
	size_t idx;

	for( idx = 0 ; idx < scene_stack.size() ; ++idx )
	{
		scene = scene_stack[ idx ].get();

		if( scene->life_cycle.is_alive() )
		{
			scene->popped();
			scene->life_cycle.set( life_cycle::dead );
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

scene* scene_mgr::get_top()
{
	assert( !scene_stack.empty() );

	return scene_stack.front().get();
}

// ----------------------------------------------------------------------------

void scene_mgr::pre_update()
{
	// remove any dead scenes
	for( size_t x = 0 ; x < scene_stack.size() ; ++x )
	{
		if( scene_stack[ x ]->life_cycle.is_dead() )
		{
			scene_stack.erase( scene_stack.begin() + x );
			x--;
		}
	}

	for( auto& iter : scene_stack )
	{
		iter->pre_update();

		if( iter->blocks_further_update )
		{
			return;
		}
	}
}

void scene_mgr::update()
{
	for( auto& iter : scene_stack )
	{
		iter->update();

		if( iter->blocks_further_update )
		{
			return;
		}
	}

	// if the game requires a game controller and one is not detected, put up
	// the dialog that waits for one to get connected.

	if( g_base_game->flags.requires_controller and !g_engine->input.gamepad )
	{
		g_engine->scenes.push<scene_controller_required>();
	}
}

void scene_mgr::post_update()
{
	for( auto& iter : scene_stack )
	{
		iter->post_update();

		if( iter->blocks_further_update )
		{
			return;
		}
	}
}

// ----------------------------------------------------------------------------

void scene_mgr::draw()
{
	if( !scene_stack.empty() )
	{
		// Locate the first completely opaque scene, starting at the top
		// and moving backward.

		int starting_scene_idx = find_first_fully_opaque_scene();

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

int scene_mgr::find_first_fully_opaque_scene()
{
	int idx = -1;

	for( const auto& iter : scene_stack )
	{
		idx++;

		if( iter->draws_completely_solid )
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

void scene_mgr::draw_scene( int starting_scene_idx )
{
	g_engine->render_api.clear_depth_buffer();

	scoped_render_state;

	for( auto x = starting_scene_idx; x >= 0; --x )
	{
		auto scene = scene_stack[ x ].get();

		if( scene->life_cycle.is_alive() )
		{
			current_scene = scene;

			render::state->z = zdepth_scene_start - ( zdepth_scene_step * x );

			g_engine->render_api.set_view_matrix_identity();

			{
				scoped_render_state;
				scene->draw();
			}

			current_scene = get_top();

		#ifndef _FINAL_RELEASE
			// draw any debug information that lives in world space.
			if( scene->is_debug_physics_scene and g_engine->renderer.debug.is_drawing_debug_info() )
			{
				render::state->z += zdepth_nudge;
				g_engine->box2d_world->DebugDraw();
			}
		#endif

			g_engine->renderer.dynamic_batches.flush_and_reset();
		}
	}
}

void scene_mgr::draw_scene_ui( int starting_scene_idx )
{
	g_engine->render_api.clear_depth_buffer();
	g_engine->render_api.set_view_matrix_identity_ui();

	scoped_render_state;

	for( auto x = starting_scene_idx; x >= 0; --x )
	{
		auto scene = scene_stack[ x ].get();

		if( scene->life_cycle.is_alive() )
		{
			render::state->z = zdepth_scene_start - ( zdepth_scene_step * x );

			// draw any screen space items, like UI. these are
			// drawn with an identity matrix so the top left of
			// the screen is always 0,0.

			render::state->z += zdepth_nudge;

			{
				scoped_opengl;

				current_scene = scene;
				g_ui->current_callback = scene->get_ui_callback();

				{
					scoped_render_state;
					scene->draw_ui();
				}

				scene->clear_expanded_tag_this_frame = false;

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
				if( g_engine->renderer.debug.is_drawing_debug_info() )
				{
					render::draw_crosshair( scene->viewport_pivot / ui_scale );
				}
			#endif
			}

			g_engine->renderer.dynamic_batches.flush_and_reset();
		}
	}
}

void scene_mgr::new_game()
{
	for( auto& scene : scene_stack )
	{
		if( scene->life_cycle.is_alive() )
		{
			scene->new_game();
		}
	}
}

bool scene_mgr::on_input_motion( const input_event* evt )
{
	for( const auto& iter : scene_stack )
	{
		if( iter->on_input_motion( evt ) )
		{
			return true;
		}

		if( iter->blocks_further_input )
		{
			return true;
		}
	}

	return false;
}

bool scene_mgr::on_input_pressed( const input_event* evt )
{
	for( const auto& iter : scene_stack )
	{
		if( iter->on_input_pressed( evt ) )
		{
			return true;
		}

		if( iter->blocks_further_input )
		{
			return true;
		}
	}

	return false;
}

bool scene_mgr::on_input_held( const input_event* evt )
{
	for( const auto& iter : scene_stack )
	{
		if( iter->on_input_held( evt ) )
		{
			return true;
		}

		if( iter->blocks_further_input )
		{
			return true;
		}
	}

	return false;
}

bool scene_mgr::on_input_released( const input_event* evt )
{
	for( const auto& iter : scene_stack )
	{
		if( iter->on_input_released( evt ) )
		{
			return true;
		}

		if( iter->blocks_further_input )
		{
			return true;
		}
	}

	return false;
}

bool scene_mgr::on_input_key( const input_event* evt )
{
	for( const auto& iter : scene_stack )
	{
		if( iter->on_input_key( evt ) )
		{
			return true;
		}

		if( iter->blocks_further_input )
		{
			return true;
		}
	}

	return false;
}

}