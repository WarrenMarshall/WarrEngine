
#include "master_pch.h"
#include "master_header.h"

w_layer::w_layer()
{
}

void w_layer::draw()
{
}

e_opaque w_layer::get_opaque_flags()
{
	return e_opaque::input;
}

void w_layer::handle_input_event( const w_input_event* evt )
{
}
