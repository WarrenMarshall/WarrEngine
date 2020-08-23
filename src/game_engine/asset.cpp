#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

bool i_asset::create_internals()
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
