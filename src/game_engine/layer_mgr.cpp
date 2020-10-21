
#include "master_pch.h"
#include "master_header.h"

w_layer_mgr::~w_layer_mgr()
{
	clear_stack();
}

void w_layer_mgr::clear_stack()
{
	// Give each layer a chance to clean up before the manager shuts down.

	for( size_t x = 0 ; x < layer_stack.size() ; ++x )
	{
		w_layer* layer = layer_stack[ x ].get();
		layer->pop();
	}

	layer_stack.clear();
}

void w_layer_mgr::pop()
{
	if( !layer_stack.size() )
	{
		log_warning( "No layers on stack - pop failed!" );
	}

	w_layer* layer = nullptr;
	int idx = 0;

	for( idx = 0 ; idx < layer_stack.size() ; ++idx )
	{
		layer = layer_stack[idx].get();

		if( layer->is_alive() )
		{
			layer->pop();
			layer->set_life_cycle( life_cycle::dead );
			break;
		}
	}

	// Since the top layer is being popped, we can tell the next layer in the stack
	// that it is being shown.
	if( layer_stack.size() > 1 )
	{
		layer = layer_stack[(size_t)idx + 1].get();
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
		if( layer_stack[x]->is_dead() )
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

	// draw starting from the starting_layer_idx and every layer above it

	RENDER->begin();

	for( auto x = starting_layer_idx; x >= 0; --x )
	{
		// Only UI elements on the topmost layer respond to user input
		UI->owning_layer_is_topmost = !x;

		if( layer_stack[x]->is_alive() )
		{
			// primary draw call for the layer. uses an optional custom camera.
			OPENGL->init_view_matrix( layer_stack[ x ]->get_camera() );
			RENDER->push_depth( zdepth_layers - ( zdepth_layer_step * x ) );
			layer_stack[ x ]->draw();

#ifdef _DEBUG
			// let the layer draw optional debug info. note that this is
			// using the optional custom camera from above, so this is
			// only for debug drawing in world space, not screen space.
			RENDER->push_depth_nudge();
			layer_stack[ x ]->draw_debug();
#endif

			// draw any screen space items, like UI. these are
			// drawn with an identity matrix so the top left of
			// the screen is always 0,0.
			OPENGL->init_view_matrix_identity();
			RENDER->push_depth_nudge();
			layer_stack[ x ]->draw_ui();

#ifdef _DEBUG
			// same as the other debug draw call, but this is for
			// info in screen space
			RENDER->push_depth_nudge(100);
			layer_stack[ x ]->draw_ui_debug();
#endif
		}
	}

	RENDER->end();
}

void w_layer_mgr::on_listener_event_received( e_event_id event, void* object )
{
	// ignore user input when engine is paused
	if( engine->is_paused )
	{
		return;
	}

	const w_input_event* evt = static_cast<w_input_event*>( object );

	for( const auto& iter : layer_stack )
	{
		if( iter->handle_input_event( evt ) )
		{
			break;
		}
	}
}
