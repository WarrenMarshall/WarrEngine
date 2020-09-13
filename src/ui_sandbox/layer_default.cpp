
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

	e_im_result res;
	w_ui_style_panel panel_style;
	panel_style.base_twk.color = w_color::light_blue;

	w_ui_style_button button_style;
	button_style.base_twk.color = w_color::dark_grey;
	button_style.label_twk.pos = w_vec2( 0, -2 );

	UI->im_passive( { 64, 64, 150, 80 }, panel_style );
	res = UI->im_active( "Options", { 80, 80, 64, 32 }, button_style );
}
