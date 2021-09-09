
#include "master_pch.h"

namespace war
{

float_t snap_to_int( float_t val )
{
	return glm::floor( val + 0.5f );
}

}
