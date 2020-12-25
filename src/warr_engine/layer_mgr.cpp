
#include "master_pch.h"
#include "master_header.h"

w_layer_mgr::~w_layer_mgr()
{
	clear_stack();
}

void w_layer_mgr::clear_stack()
{
	// Give each layer a chance to clean up before the manager shuts down.

	for( const auto& layer : layer_stack )
	{
		layer->pop();
	}

	layer_stack.clear();
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
	// Locate the first completely solid layer, starting at the top
	// and moving backward.

	int starting_layer_idx = 0;
	for( const auto& iter : layer_stack )
	{
		if( iter->draws_completely_solid )
		{
			break;
		}

		starting_layer_idx++;
	}

	// clear the virtual viewport with a full size rectangle

	RENDER
		->begin()
		->push_depth( zdepth_clear_window )
		->push_rgb( engine->window->v_window_clear_color )
		->draw_filled_rectangle( w_rect( 0, 0, v_window_w, v_window_h ) )
		->end();

	if( !layer_stack.empty() )
	{
		// draw starting from the starting_layer_idx and every layer above it

		RENDER->begin();

		for( auto x = starting_layer_idx; x >= 0; --x )
		{
			auto layer = layer_stack[ x ].get();

			// Only UI elements on the topmost layer respond to user input
			IMGUI->containing_layer_is_topmost = !x;

			if( layer->ilc_is_alive() )
			{
				w_camera* camera = layer->get_camera();

				// draw the layer. uses an optional custom camera.
				{
					OPENGL->init_view_matrix( camera );
					RENDER->push_depth( zdepth_layers - ( zdepth_layer_step * x ) );
					layer->draw();
				}

				// draw any debug information that lives in world space.
				{
					if( RENDER->show_physics_debug && layer->is_topmost_layer() )
					{
						// box2d needs the origin of the world to be in the middle of the viewport

						// #todo - needs testing with a camera in place and all that

						if( camera )
						{
							MATRIX
								->push()
								->translate( w_vec2( v_window_hw, v_window_hh ) );
						}

						RENDER->push_depth_nudge();
						engine->box2d_world->DebugDraw();

						if( camera )
						{
							MATRIX->pop();
						}
					}
				}

				// draw any screen space items, like UI. these are
				// drawn with an identity matrix so the top left of
				// the screen is always 0,0.
				{
					OPENGL->init_view_matrix_identity_ui();
					RENDER->push_depth_nudge();
					layer->draw_ui();
				}
			}
		}

		RENDER->end();
	}
}

bool w_layer_mgr::iir_on_motion( const w_input_event* evt )
{
	for( const auto& iter : layer_stack )
	{
		if( iter->iir_on_motion( evt ) )
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

bool w_layer_mgr::iir_on_pressed( const w_input_event* evt )
{
	for( const auto& iter : layer_stack )
	{
		if( iter->iir_on_pressed( evt ) )
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

bool w_layer_mgr::iir_on_held( const w_input_event* evt )
{
	for( const auto& iter : layer_stack )
	{
		if( iter->iir_on_held( evt ) )
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

bool w_layer_mgr::iir_on_released( const w_input_event* evt )
{
	for( const auto& iter : layer_stack )
	{
		if( iter->iir_on_released( evt ) )
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
