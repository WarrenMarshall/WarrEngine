
#include "master_pch.h"
#include "master_header.h"

namespace war
{

bool Gradient_Source_Asset::create()
{
	// gradients are 1x? or ?x1 textures
	//
	// the length of a gradient is determined by the alignment specified in the
	// asset_def file. they are a list of colors either running vertically or
	// horizontally, which implies the other dimension is 1 pixel in size.

	w = (float)( ( alignment == align::horizontal ) ? ( colors.size() / 4 ) : 1 );
	h = (float)( ( alignment == align::vertical ) ? ( colors.size() / 4 ) : 1 );

	assert( colors.size() == w * h * 4 );

	glCreateTextures( GL_TEXTURE_2D, 1, &gl_id );
	glBindTextureUnit( 0, gl_id );

	glTextureParameteri( gl_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR );
	glTextureParameteri( gl_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	glTextureParameteri( gl_id, GL_TEXTURE_WRAP_S, ( alignment == align::vertical ) ? GL_REPEAT : GL_CLAMP_TO_EDGE );
	glTextureParameteri( gl_id, GL_TEXTURE_WRAP_T, ( alignment == align::horizontal ) ? GL_REPEAT : GL_CLAMP_TO_EDGE );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, (GLsizei)w, (GLsizei)h, 0, GL_RGBA, GL_FLOAT, colors.data() );

	glGenerateMipmap( GL_TEXTURE_2D );

	return true;
}

}
