
#include "master_pch.h"
#include "master_header.h"

void w_layer_mgr::push( std::unique_ptr<w_layer> layer )
{
	w_layer* top = get_top();
	if( top )
	{
		top->getting_covered();
	}

	layer->push();
	layer->becoming_top_layer();

	layer_stack.insert( layer_stack.begin(), std::move( layer ) );
}

void w_layer_mgr::pop()
{
	w_layer* layer = nullptr;
	int idx = 0;

	for( idx = 0 ; idx < layer_stack.size() ; ++idx )
	{
		layer = layer_stack[idx].get();

		if( layer->is_alive() )
		{
			layer->getting_covered();
			layer->set_life_cycle( lifecycle::dead );
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
	else
	{
		log_msg( "layer_mgr::pop : no layers on stack - pop failed!" );
	}
}

w_layer* w_layer_mgr::get_top()
{
	if( layer_stack.empty() )
	{
		return nullptr;
	}

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

	// then draw that layer and every layer above it

	RENDER->begin();

	//RENDER
		//->push_rgb( engine->window->v_window_clear_color )
		//->draw_filled_rectangle( w_rect( 0, 0, v_window_w, v_window_h ) );

	for( int x = starting_layer_idx; x >= 0; --x )
	{
		// Only UI elements on the topmost layer respond to user input
		UI->owning_layer_is_topmost = !x;

		if( layer_stack[x]->is_alive() )
		{
			RENDER->push_depth( zdepth_layers - ( zdepth_layer_step * x ) );
			layer_stack[ x ]->draw();
		}
	}

	RENDER->end();
}

void w_layer_mgr::on_listener_event_received( e_event_id event, void* object )
{
	const w_input_event* evt = static_cast<w_input_event*>( object );

	for( const auto& iter : layer_stack )
	{
		if( iter->handle_input_event( evt ) )
		{
			break;
		}
	}
}
