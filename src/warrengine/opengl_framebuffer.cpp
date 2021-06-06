
#include "master_pch.h"
#include "master_header.h"

w_opengl_framebuffer::w_opengl_framebuffer( const std::string& base_name )
	: base_name( base_name )
{
	glCreateFramebuffers( 1, &fb_id );

	bind();
}

void w_opengl_framebuffer::add_color_attachment( const w_color& clear_color )
{
	color_attachments.emplace_back( w_opengl_color_attachment( this, clear_color ) );
}

void w_opengl_framebuffer::finalize()
{
	if( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
	{
		log_error( "Frame buffer is incomplete!" );
	}

	// tell opengl to render to all color attachments (by default, it only renders to the first)

	std::vector<unsigned> ids;

	for( unsigned id = 0 ; id < color_attachments.size() ; ++id )
	{
		ids.push_back( GL_COLOR_ATTACHMENT0 + id );
	}

	glDrawBuffers( static_cast<int>( color_attachments.size() ), ids.data() );

	unbind();
}

w_opengl_framebuffer::~w_opengl_framebuffer()
{
	unbind();

	if( fb_id )
	{
		glDeleteFramebuffers( 1, &fb_id );
	}

	glBindRenderbuffer( GL_RENDERBUFFER, 0 );
	glBindTextureUnit( 0, 0 );
}

void w_opengl_framebuffer::bind()
{
	glBindFramebuffer( GL_FRAMEBUFFER, fb_id );
}

void w_opengl_framebuffer::unbind()
{
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}
