#include "master_pch.h"
#include "master_header.h"

a_texture::~a_texture()
{
	a_texture::clean_up_internals();
}

void a_texture::clean_up_internals()
{
	//render_buffer = nullptr;

	if( id )
	{
		glDeleteTextures( 1, &id );
	}
}

bool a_texture::create_internals()
{
	assert( !original_filename.empty() );

	auto file = engine->fs->load_file_into_memory( original_filename );
	int width, height, bpp;
	unsigned char* color_data = stbi_load_from_memory(
		(const stbi_uc*) ( file->buffer->data() ),
		static_cast<int>( file->buffer->size() ),
		&width, &height, &bpp, 4 );
	w = static_cast<float>( width );
	h = static_cast<float>( height );

	if( !color_data )
	{
		log_error( "Couldn't load the file : [{}]", original_filename );
	}

	// upload texture to opengl

	glGenTextures( 1, &id );
	bind();

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, color_data );
	OPENGL->clear_texture_bind();

	stbi_image_free( color_data );
	color_data = nullptr;

	return true;
}

void a_texture::bind()
{
	glBindTexture( GL_TEXTURE_2D, id );
}

void a_texture::unbind()
{
	glBindTexture( GL_TEXTURE_2D, 0 );
}

a_subtexture* a_texture::get_subtexture( float offset )
{
	return subtex;
}

