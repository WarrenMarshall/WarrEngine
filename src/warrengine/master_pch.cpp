
#include "master_pch.h"

namespace war
{

float sign_of( float val )
{
	return glm::sign( val );
}

float sin_from_angle( float angle_in_degrees )
{
	return glm::sin( glm::radians( angle_in_degrees ) );
}

}
