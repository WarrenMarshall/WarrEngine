
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

	layer_stack.push_front( std::move( layer ) );
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
			layer->set_life_cycle( e_lifecycle::dead );
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
	if( layer_stack.size() == 0 )
	{
		return nullptr;
	}

	return layer_stack.front().get();
}

void w_layer_mgr::update()
{
	// remove any dead layers
	for( auto iter = layer_stack.begin(); iter != layer_stack.end(); iter++ )
	{
		if( ( *iter )->is_dead() )
		{
			layer_stack.erase( iter-- );
		}
	}

	// update the living layers
	for( auto& iter : layer_stack )
	{
		iter->update();
	}
}

w_layer* w_layer_mgr::find_topmost_input_listener()
{
	return nullptr;
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

	for( int x = starting_layer_idx; x >= 0; --x )
	{
		if( layer_stack[x]->is_alive() )
		{
			layer_stack[x]->draw();
		}
	}
}

void w_layer_mgr::on_listener_event_received( e_event_id event, void* object )
{
	w_layer* layer = find_topmost_input_listener();

	if( !layer )
	{
		return;
	}

	const w_input_event* evt = static_cast<w_input_event*>( object );
	layer->handle_input_event( evt );
}
