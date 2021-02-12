
#include "master_pch.h"
#include "master_header.h"

w_opengl_color_attachment::w_opengl_color_attachment( w_opengl_framebuffer* fb_owner )
	: fb_owner( fb_owner )
{
	// figure out how to identify the internal texture
	int texture_num = (int)fb_owner->color_attachments.size();
	std::string tex_name = fmt::format( "tex{}_{}_frame_buffer", texture_num, fb_owner->base_name );

	// create the internal texture
	engine->asset_cache->add( std::make_unique<a_src_texture>(), tex_name, "" );
	texture = engine->asset_cache->add( std::make_unique<a_texture>( tex_name ), "fb_" + tex_name, "" );
	texture->src_texture->w = v_window_w;
	texture->src_texture->h = v_window_h;

	// create the opengl texture
	glCreateTextures( GL_TEXTURE_2D, 1, &texture->src_texture->gl_id );
	glBindTextureUnit( 0, texture->src_texture->gl_id );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA16F, (int)v_window_w, (int)v_window_h, 0, GL_RGBA, GL_FLOAT, nullptr );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	// attach to the owning frame buffer
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + texture_num, GL_TEXTURE_2D, texture->src_texture->gl_id, 0 );
}

// ----------------------------------------------------------------------------

w_opengl_framebuffer::w_opengl_framebuffer( const std::string& base_name )
	: base_name( base_name )
{
	glCreateFramebuffers( 1, &fb_id );

	bind();
}

void w_opengl_framebuffer::add_color_attachment()
{
	color_attachments.emplace_back( w_opengl_color_attachment( this ) );
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
