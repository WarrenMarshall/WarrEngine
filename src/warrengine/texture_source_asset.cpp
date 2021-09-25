
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
	w = (float_t)sz.x;
	h = (float_t)sz.y;

	image->flipVertically();

	glCreateTextures( GL_TEXTURE_2D, 1, &gl_id );
	glBindTextureUnit( 0, gl_id );

	auto filter = GL_NEAREST;
	auto filter_mipmap = GL_LINEAR_MIPMAP_NEAREST;

	if( use_linear_filtering )
	{
		filter = GL_LINEAR;
		filter_mipmap = GL_LINEAR_MIPMAP_LINEAR;
	}

	glTextureParameteri( gl_id, GL_TEXTURE_MIN_FILTER, ( use_mipmaps ) ? filter_mipmap : filter );
	glTextureParameteri( gl_id, GL_TEXTURE_MAG_FILTER, filter );

	glTextureParameteri( gl_id, GL_TEXTURE_WRAP_S, ( tiling == e_tiling::clamp ) ? GL_CLAMP_TO_EDGE : GL_REPEAT );
	glTextureParameteri( gl_id, GL_TEXTURE_WRAP_T, ( tiling == e_tiling::clamp ) ? GL_CLAMP_TO_EDGE : GL_REPEAT );

	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA8,
		(int32_t)w, (int32_t)h,
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
