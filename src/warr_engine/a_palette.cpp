
#include "master_pch.h"
#include "master_header.h"

implement_find_func( a_palette )

w_color a_palette::get_color_from_idx( int idx )
{
	assert( idx < colors.size() );

	return colors[ idx ];
}
