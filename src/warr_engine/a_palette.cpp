
#include "master_pch.h"
#include "master_header.h"

implement_find_func( a_palette )

a_palette::~a_palette()
{
	a_palette::clean_up_internals();
}

w_color a_palette::get_color_from_idx( int idx )
{
	assert( idx < colors.size() );

	return colors[ idx ];
}

void a_palette::clean_up_internals()
{
}

bool a_palette::create_internals()
{
	return true;
}
