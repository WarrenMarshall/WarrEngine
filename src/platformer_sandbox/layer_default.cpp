
#include "app_header.h"

layer_default::layer_default()
{
	draws_completely_solid = true;
}

void layer_default::push()
{
}

void layer_default::draw()
{
	w_layer::draw();
}
