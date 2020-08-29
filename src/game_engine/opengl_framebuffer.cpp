
#include "master_pch.h"
#include "master_header.h"

w_opengl_framebuffer::w_opengl_framebuffer()
{
	glGenFramebuffers( 1, &fb_id );

	bind();

	// color buffer

	glGenTextures( 1, &tex_id );
	glBindTexture( GL_TEXTURE_2D, tex_id );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, (int) v_window_w, (int) v_window_h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_id, 0 );

	// depth/stencil buffer

	glGenRenderbuffers( 1, &rbo );
	glBindRenderbuffer( GL_RENDERBUFFER, rbo );
	glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, (int) v_window_w, (int) v_window_h );
	glBindRenderbuffer( GL_RENDERBUFFER, 0 );

	glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo );

	if( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
	{
		log_error( fmt::format( "{} : failed to create frame buffer", __FUNCTION__ ) );
	}

	unbind();
}

w_opengl_framebuffer::~w_opengl_framebuffer()
{
	if( fb_id )
	{
		glDeleteFramebuffers( 1, &fb_id );
	}
	if( tex_id )
	{
		glDeleteTextures( 1, &tex_id );
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
