
#include "master_pch.h"
#include "master_header.h"

implement_find_func( a_gradient )

a_gradient::~a_gradient()
{
	a_gradient::clean_up_internals();
}

void a_gradient::clean_up_internals()
{
}

bool a_gradient::create_internals()
{
	// gradients are 1x? or ?x1 textures
	//
	// the length of a gradient is determined by the alignment specified in the asset_def
	// file. they are a list of colors either running vertically or horizontally, which
	// implies the other dimension is 1 pixel in size.

	w = static_cast<float>( ( alignment == align::horizontal ) ? ( colors.size() / 4 ) : 1 );
	h = static_cast<float>( ( alignment == align::vertical ) ? ( colors.size() / 4 ) : 1 );

	return true;
}

void a_gradient::finalize_after_loading()
{
	glCreateTextures( GL_TEXTURE_2D, 1, &gl_id );
	glBindTextureUnit( 0, gl_id );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, (GLsizei) w, (GLsizei) h, 0, GL_RGBA, GL_FLOAT, colors.data() );
}
