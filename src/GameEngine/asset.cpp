#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

void i_asset::clean_up_internals()
{
}

bool i_asset::create_internals( bool is_hot_reloading )
{
	return false;
}

void i_asset::update_fts()
{
}

void i_asset::draw( e_render_pass render_pass )
{
}

void i_asset::clear_render_buffer()
{
	if( render_buffer )
		render_buffer->clear();
}
