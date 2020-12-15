
#include "master_pch.h"
#include "master_header.h"

w_opengl_framebuffer::w_opengl_framebuffer( const std::string& base_name, float w, float h )
	: w( w ), h( h )
{
	glGenFramebuffers( 1, &fb_id );

	bind();

	std::string tex_name = fmt::format( "tex0_{}_frame_buffer", base_name );

	// color buffer 0

	tex0 = engine->asset_cache->add( std::make_unique<a_texture>(), tex_name, "" );
	tex0->w = w;
	tex0->h = h;

	glGenTextures( 1, &tex0->gl_id );
	glBindTexture( GL_TEXTURE_2D, tex0->gl_id );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, (int) w, (int) h, 0, GL_RGBA, GL_FLOAT, nullptr );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex0->gl_id, 0 );

	tex0->subtex = engine->asset_cache->add( std::make_unique<a_subtexture>( tex_name ), "sub_" + tex_name, "" );

	// color buffer 1

	tex_name = fmt::format( "tex1_{}_frame_buffer", base_name );

	tex1 = engine->asset_cache->add( std::make_unique<a_texture>(), tex_name, "" );
	tex1->w = w;
	tex1->h = h;

	glGenTextures( 1, &tex1->gl_id );
	glBindTexture( GL_TEXTURE_2D, tex1->gl_id );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, (int) w, (int) h, 0, GL_RGBA, GL_FLOAT, nullptr );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, tex1->gl_id, 0 );

	tex1->subtex = engine->asset_cache->add( std::make_unique<a_subtexture>( tex_name ), "sub_" + tex_name, "" );

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

	// tell opengl to render to both color buffers (by default, it only renders to the first)

	unsigned int ids[ 2 ] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers( 2, ids );

	unbind();
}

w_opengl_framebuffer::~w_opengl_framebuffer()
{
	if( fb_id )
	{
		glDeleteFramebuffers( 1, &fb_id );
	}
	if( tex0->gl_id )
	{
		glDeleteTextures( 1, &tex0->gl_id );
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
