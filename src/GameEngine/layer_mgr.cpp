
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
	for( auto iter = layer_stack.begin(); iter != layer_stack.end(); iter++ )
	{
		if( ( *iter )->is_dead() )
		{
			layer_stack.erase( iter-- );
		}
	}

	for( auto& iter : layer_stack )
	{
		iter->update();

		// If this layer is completely opaque, don't bother updating the
		// layers below it - they are not visible
		if( ( iter->get_opaque_flags() & e_opaque::draw ) > 0 )
		{
			break;
		}
	}
}

/*
	loops through the layer stack, front to back, and returns
	the first layer index that is e_opaque::draw
*/
int w_layer_mgr::get_opaque_index( e_opaque opaque_flags )
{
	/*
		drawing is a back-to-front sequence.

		1. iterate forwards in the stack until an opaque layer is found
		2. iterate backwards from there until back at the start of the stack
	*/
	int opaque_index = -1;
	for( const auto& iter : layer_stack )
	{
		opaque_index++;

		if( (iter->get_opaque_flags() & opaque_flags) > 0 )
		{
			break;
		}
	}

#if _DEBUG
	if( opaque_index == -1 )
		log_error( "layer_mgr::draw : no opaque layers found!" );
#endif

	return opaque_index;
}

/*
	called once per frame to:
		- let active layers draw
*/
void w_layer_mgr::draw()
{
	int starting_layer_idx = get_opaque_index( e_opaque::draw );

	for( int x = starting_layer_idx; x >= 0; --x )
	{
		if( layer_stack[x]->is_alive() )
		{
			layer_stack[x]->draw();
		}
	}
}
