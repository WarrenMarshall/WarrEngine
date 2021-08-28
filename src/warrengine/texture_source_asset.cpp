
#include "master_pch.h"
#include "master_header.h"

namespace war
{

bool Texture_Source_Asset::create()
{
	assert( !original_filename.empty() );

	auto file = File_System::load_binary_file( original_filename );

	// for safety, allocate the image on the heap - loading a large texture
	// might be bad news on the stack
	auto image = std::make_unique<sf::Image>();

	if( !image->loadFromMemory( file->buffer.data(), file->buffer.size() ) )
	{
		log_fatal( "Couldn't load the file : [{}]", original_filename );
	}

	auto sz = image->getSize();
	w = (float)sz.x;
	h = (float)sz.y;

	image->flipVertically();

	glCreateTextures( GL_TEXTURE_2D, 1, &gl_id );
	glBindTextureUnit( 0, gl_id );

	glTextureParameteri( gl_id, GL_TEXTURE_MIN_FILTER, use_mipmaps ? GL_LINEAR_MIPMAP_NEAREST : GL_NEAREST );
	glTextureParameteri( gl_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

	if( tiling == e_tiling::clamp )
	{
		glTextureParameteri( gl_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTextureParameteri( gl_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	}
	else
	{
		glTextureParameteri( gl_id, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTextureParameteri( gl_id, GL_TEXTURE_WRAP_T, GL_REPEAT );
	}

	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA8,
		(int)( w ), (int)( h ),
		0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)( image->getPixelsPtr() ) );

	if( use_mipmaps )
	{
		// even though we want chunky pixels, having mipmaps still looks better when
		// scaling quads and sprites - not using mipmaps looks pretty bad

		glGenerateMipmap( GL_TEXTURE_2D );
	}

	return true;
}

}
