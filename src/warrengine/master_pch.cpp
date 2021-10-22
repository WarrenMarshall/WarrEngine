
#include "master_pch.h"

namespace war
{

f32 snap_to_int( f32 val )
{
	return glm::floor( val + ( 0.5f * glm::sign( val ) ) );
}

}
