
#include "master_pch.h"
#include "master_header.h"

void i_asset::clean_up_internals()
{
}

bool i_asset::create_internals()
{
	return false;
}

void i_asset::finalize_after_loading()
{
	// assets should use this function to do things that aren't
	// thread-safe ... like uploading textures to OpenGL
}
