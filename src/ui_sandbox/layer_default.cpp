
#include "app_header.h"

// ----------------------------------------------------------------------------

layer_default::layer_default()
{
	draws_completely_solid = true;
}

void layer_default::push()
{
	gradient = engine->get_asset<a_gradient>( "background_gradient" );
}

void layer_default::draw()
{
	w_layer::draw();

	if( UI->im_push_button( "Push Button", w_rect( 8, 8, 200, 32 ) ).was_left_clicked() )
	{
		log_msg( "button clicked" );
	}

	RENDER
		->begin()
		->draw( gradient, w_rect( 0, 0, v_window_w, v_window_h ) )
		->push_rgb( w_color::white )
		->push_align( align::centered )
		->draw_string( engine->pixel_font, "Vices and Vacations, Venom and Vuum.", w_rect( v_window_hw, v_window_hh ) )
		->draw_string( engine->pixel_font, "The victory was mine, as was the gold!", w_rect( v_window_hw, v_window_hh + engine->pixel_font->font_def->max_height ) )
		->draw_string( engine->pixel_font, "Never underestimate the power of a PIRATE!", w_rect( v_window_hw, v_window_hh + ( engine->pixel_font->font_def->max_height * 2 ) ) )
		->end();
}
