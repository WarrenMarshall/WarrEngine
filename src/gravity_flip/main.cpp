
#include "app_header.h"

using namespace war;

int main( int argc, char* argv [] )
{
	engine::go<gravity_flip_game>( argc, argv );

	return EXIT_SUCCESS;
}
