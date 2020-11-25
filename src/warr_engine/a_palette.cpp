
#include "master_pch.h"
#include "master_header.h"

implement_find_func( a_palette )

a_palette::~a_palette()
{
	a_palette::clean_up_internals();
}

void a_palette::clean_up_internals()
{

}

bool a_palette::create_internals()
{
	return true;
}

