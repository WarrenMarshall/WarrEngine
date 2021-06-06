
#include "app_header.h"

using namespace war;

int main( int argc, char* argv [] )
{
	engine::go<tpl_asteroids_game>( argc, argv );

	return EXIT_SUCCESS;
}
