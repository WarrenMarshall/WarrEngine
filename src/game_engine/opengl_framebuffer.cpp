
#include "master_pch.h"
#include "master_header.h"

w_opengl_framebuffer::w_opengl_framebuffer( const std::string& base_name, float w, float h )
	: w( w ), h( h )
{
	glGenFramebuffers( 1, &fb_id );

	bind();

	// color buffer

	std::string tex_name = "tex_" + base_name + "_frame_buffer";

	tex = engine->asset_cache->add( std::make_unique<a_texture>(), tex_name, "" );

	tex->w = w;
	tex->h = h;

	glGenTextures( 1, &tex->id );
	glBindTexture( GL_TEXTURE_2D, tex->id );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, (int) w, (int) h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->id, 0 );

	tex->subtex = engine->asset_cache->add( std::make_unique<a_subtexture>( tex_name ), "sub_" + tex_name, "" );

	// depth/stencil buffer

	glGenRenderbuffers( 1, &rbo );
	glBindRenderbuffer( GL_RENDERBUFFER, rbo );
	glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, (int) w, (int) h );
	glBindRenderbuffer( GL_RENDERBUFFER, 0 );

	glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo );

	if( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
	{
		log_error( "Failed to create frame buffer" );
	}

	unbind();
}

w_opengl_framebuffer::~w_opengl_framebuffer()
{
	if( fb_id )
	{
		glDeleteFramebuffers( 1, &fb_id );
	}
	if( tex->id )
	{
		glDeleteTextures( 1, &tex->id );
	}
}

void w_opengl_framebuffer::bind()
{
	glBindFramebuffer( GL_FRAMEBUFFER, fb_id );
}

void w_opengl_framebuffer::unbind()
{
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}
