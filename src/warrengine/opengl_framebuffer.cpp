
#include "master_pch.h"
#include "master_header.h"

namespace war
{

opengl_framebuffer::opengl_framebuffer( std::string_view base_name )
	: base_name( base_name.data() )
{
	glCreateFramebuffers( 1, &fb_id );

	bind();
}

void opengl_framebuffer::add_color_attachment( vec2 sz, const color& clear_color )
{
	color_attachments.emplace_back( opengl_color_attachment( this, sz, clear_color ) );
}

void opengl_framebuffer::add_depth_attachment( vec2 sz )
{
	// depth/stencil buffer

	glCreateRenderbuffers( 1, &rbo_id );
	glBindRenderbuffer( GL_RENDERBUFFER, rbo_id );
	glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, (int)sz.w, (int)sz.h );
	glBindRenderbuffer( GL_RENDERBUFFER, 0 );

	glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_id );
}

void opengl_framebuffer::finalize()
{
	auto status = glCheckFramebufferStatus( GL_FRAMEBUFFER );

	if( status != GL_FRAMEBUFFER_COMPLETE )
	{
		switch( status )
		{
			case GL_FRAMEBUFFER_UNDEFINED:
			log_fatal( "GL_FRAMEBUFFER_UNDEFINED" );
			break;
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			log_fatal( "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT" );
			break;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			log_fatal( "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT" );
			break;
			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			log_fatal( "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER" );
			break;
			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			log_fatal( "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER" );
			break;
			case GL_FRAMEBUFFER_UNSUPPORTED:
			log_fatal( "GL_FRAMEBUFFER_UNSUPPORTED" );
			break;
			case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
			log_fatal( "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE" );
			break;
			case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
			log_fatal( "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS" );
			break;
			default:
			log_fatal( "glCheckFramebufferStatus failed" );
			break;
		}
	}

	// tell opengl to render to all color attachments (by default, it only renders to the first)

	std::vector<unsigned> ids;

	for( unsigned id = 0 ; id < color_attachments.size() ; ++id )
	{
		ids.push_back( GL_COLOR_ATTACHMENT0 + id );
	}

	glDrawBuffers( (int)( color_attachments.size() ), ids.data() );

	unbind();
}

opengl_framebuffer::~opengl_framebuffer()
{
	unbind();

	if( fb_id )
	{
		glDeleteFramebuffers( 1, &fb_id );
	}

	if( rbo_id )
	{
		glDeleteRenderbuffers( 1, &rbo_id );
	}

	glBindRenderbuffer( GL_RENDERBUFFER, 0 );
	glBindTextureUnit( 0, 0 );
}

void opengl_framebuffer::bind()
{
	glBindFramebuffer( GL_FRAMEBUFFER, fb_id );
}

void opengl_framebuffer::unbind()
{
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

void opengl_framebuffer::clear_color_attachments()
{
	for( auto& ca : color_attachments )
	{
		ca.clear();
	}
}

}
