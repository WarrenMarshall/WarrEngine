
#include "master_pch.h"
#include "master_header.h"

implement_find_func( a_src_texture )

a_src_texture::~a_src_texture()
{
	a_src_texture::clean_up_internals();
}

void a_src_texture::clean_up_internals()
{
	if( gl_id )
	{
		glDeleteTextures( 1, &gl_id );
		gl_id = 0;
	}
}

bool a_src_texture::create_internals()
{
	assert( !original_filename.empty() );

	auto file = engine->fs->load_binary_file( original_filename );
	image = std::make_unique<sf::Image>();
	if( !image->loadFromMemory( file->buffer->data(), file->buffer->size() ) )
	{
		image = nullptr;
		log_error( "Couldn't load the file : [{}]", original_filename );
	}

	auto sz = image->getSize();
	w = (float)sz.x;
	h = (float)sz.y;

	image->flipVertically();

	return true;
}

void a_src_texture::finalize_after_loading()
{
	assert( image );

	glCreateTextures( GL_TEXTURE_2D, 1, &gl_id );
	glBindTextureUnit( 0, gl_id );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

	if( tiling == tiling::clamp )
	{
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	}
	else
	{
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	}

	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA8,
		static_cast<int>( w ), static_cast<int>( h ),
		0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)( image->getPixelsPtr() ) );

	// free the source data now that it's been uploaded to opengl
	image = nullptr;
}
