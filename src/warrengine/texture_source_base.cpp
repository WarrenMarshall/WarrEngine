
#include "master_pch.h"

namespace war
{

Texture_Source_Base::~Texture_Source_Base()
{
	if( gl_id )
	{
		glDeleteTextures( 1, &gl_id );
		gl_id = 0;
	}
}

}
