
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
	}
}

bool a_src_texture::create_internals()
{
	assert( !original_filename.empty() );

	auto file = engine->fs->load_binary_file( original_filename );
	int width, height, bpp;
	color_data = stbi_load_from_memory(
		(const stbi_uc*) ( file->buffer->data() ),
		static_cast<int>( file->buffer->size() ),
		&width, &height, &bpp, 4 );
	w = static_cast<float>( width );
	h = static_cast<float>( height );

	if( !color_data )
	{
		log_error( "Couldn't load the file : [{}]", original_filename );
	}

	return true;
}

void a_src_texture::finalize_after_loading()
{
	glCreateTextures( GL_TEXTURE_2D, 1, &gl_id );
	glBindTextureUnit( 0, gl_id );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA8,
		static_cast<int>( w ), static_cast<int>( h ),
		0, GL_RGBA, GL_UNSIGNED_BYTE, color_data );

	// free the source data now that it's been uploaded to opengl

	stbi_image_free( color_data );
	color_data = nullptr;
}
