
#include "master_pch.h"

namespace war
{

float snap_to_int( float val )
{
	return glm::floor( val + 0.5f );
}

}
