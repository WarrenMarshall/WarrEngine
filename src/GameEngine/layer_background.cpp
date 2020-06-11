
#include "master_pch.h"
#include "master_header.h"

void layer_background::draw()
{
	w_layer::draw();
}

e_opaque layer_background::get_opaque_flags()
{
	return e_opaque::draw;
}
