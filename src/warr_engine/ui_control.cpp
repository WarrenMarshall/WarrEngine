#include "master_pch.h"
#include "master_header.h"

w_offset w_imgui_control::get_base_offset()
{
	if( slice_def )
	{
		return w_offset( 0, -2 );
	}

	return w_offset( 0, 0 );
}
