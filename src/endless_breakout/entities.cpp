
#include "app_header.h"

e_background_stars::e_background_stars()
{
	add_component<ec_emitter>()->init( "background_stars" );
}
