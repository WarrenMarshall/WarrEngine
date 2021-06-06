
#include "master_pch.h"
#include "master_header.h"

namespace war
{

texture_source_base::~texture_source_base()
{
	if( gl_id )
	{
		glDeleteTextures( 1, &gl_id );
		gl_id = 0;
	}
}

}
