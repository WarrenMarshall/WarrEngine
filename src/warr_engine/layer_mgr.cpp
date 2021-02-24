
#include "master_pch.h"
#include "master_header.h"

void w_layer_mgr::clear_stack()
{
	// Give each layer a chance to clean up before the manager shuts down.

	for( const auto& layer : layer_stack )
	{
		layer->getting_covered();
		layer->pop();
		layer->ilc_set( life_cycle::dead );
	}
}

void w_layer_mgr::pop()
{
	if( layer_stack.empty() )
	{
		log_warning( "No layers on stack - pop failed!" );
	}

	w_layer* layer = nullptr;
	size_t idx;

	for( idx = 0 ; idx < layer_stack.size() ; ++idx )
	{
		layer = layer_stack[idx].get();

		if( layer->ilc_is_alive() )
		{
			layer->pop();
			layer->ilc_set( life_cycle::dead );
			break;
		}
	}

	// Since the top layer is being popped, we can tell the next layer in the stack
	// that it is being shown.
	if( layer_stack.size() > 1 )//&& ( idx + 1 ) < layer_stack.size() )
	{
		layer = layer_stack[idx + 1].get();
		layer->becoming_top_layer();
	}
}

w_layer* w_layer_mgr::get_top()
{
	assert( !layer_stack.empty() );

	return layer_stack.front().get();
}

void w_layer_mgr::update()
{
	// remove any dead layers
	for( size_t x = 0 ; x < layer_stack.size() ; ++x )
	{
		if( layer_stack[x]->ilc_is_dead() )
		{
			layer_stack.erase( layer_stack.begin() + x );
			x--;
		}
	}

	// update the living layers

	for( auto& iter : layer_stack )
	{
		iter->update();
	}
}

void w_layer_mgr::draw()
{
	if( !layer_stack.empty() )
	{
		// Locate the first completely solid layer, starting at the top
		// and moving backward.

		int starting_layer_idx = 0;
		bool found_opaque_layer = false;
		for( const auto& iter : layer_stack )
		{
			if( iter->draws_completely_solid )
			{
				found_opaque_layer = true;
				break;
			}

			starting_layer_idx++;
		}

		// if we never find an opaque layer something has been set up
		// wrong. make sure that at least 1 layer in the stack is marked as
		// "draws_completely_solid = true"
		assert( found_opaque_layer );

		// draw starting from the starting_layer_idx and every layer above it

		// ----------------------------------------------------------------------------
		// DRAW
		// ----------------------------------------------------------------------------

		{
			scoped_render_state;

			for( auto x = starting_layer_idx; x >= 0; --x )
			{
				auto layer = layer_stack[ x ].get();

				if( layer->ilc_is_alive() )
				{
					render_state.z = zdepth_layers - ( zdepth_layer_step * x );
					engine->opengl->init_view_matrix( layer->get_camera() );

					layer->draw();

					// draw any debug information that lives in world space.
					if( engine->render->show_extra_debug && layer->is_debug_physics_layer )
					{
						render_state.z += zdepth_nudge;
						engine->box2d_world->DebugDraw();
					}

					engine->render->draw_and_reset_all_batches();
				}
			}
		}

		// ----------------------------------------------------------------------------
		// UI
		// ----------------------------------------------------------------------------

		{
			scoped_render_state;

			render_state.z = zdepth_clear_window;

			engine->opengl->init_view_matrix_identity_ui();

			for( auto x = starting_layer_idx; x >= 0; --x )
			{
				auto layer = layer_stack[ x ].get();

				// Only UI elements on the topmost layer respond to user input
				engine->ui->imgui->containing_layer_is_topmost = !x;

				if( layer->ilc_is_alive() )
				{
					render_state.z = zdepth_layers - ( zdepth_layer_step * x );

					// draw any screen space items, like UI. these are
					// drawn with an identity matrix so the top left of
					// the screen is always 0,0.

					render_state.z += zdepth_nudge;

					{
						scoped_opengl;

						engine->ui->imgui->current_layer = layer;
						layer->draw_ui();
						engine->ui->imgui->current_layer = nullptr;

						// when the final layer has been drawn, add anything on top that
						// we need to - like a mouse cursor - to contain it within the same draw call.
						if( !x )
						{
							engine->ui->draw_topmost();
						}
					}

					engine->render->draw_and_reset_all_batches();
				}
			}
		}
	}
}

void w_layer_mgr::new_game()
{
	for( auto& layer : layer_stack )
	{
		if( layer->ilc_is_alive() )
		{
			layer->new_game();
		}
	}
}

bool w_layer_mgr::on_input_motion( const w_input_event* evt )
{
	for( const auto& iter : layer_stack )
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

bool w_layer_mgr::on_input_pressed( const w_input_event* evt )
{
	for( const auto& iter : layer_stack )
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

bool w_layer_mgr::on_input_held( const w_input_event* evt )
{
	for( const auto& iter : layer_stack )
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

bool w_layer_mgr::on_input_released( const w_input_event* evt )
{
	for( const auto& iter : layer_stack )
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

bool w_layer_mgr::on_input_key( const w_input_event* evt )
{
	for( const auto& iter : layer_stack )
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
