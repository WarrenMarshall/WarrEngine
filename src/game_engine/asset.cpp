#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

bool i_asset::create_internals( bool is_hot_reloading )
{
	return false;
}

void i_asset::clear_render_buffer()
{
	if( render_buffer )
	{
		render_buffer->clear();
	}
}
