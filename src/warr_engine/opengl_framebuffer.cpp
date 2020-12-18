
#include "master_pch.h"
#include "master_header.h"

w_opengl_framebuffer::w_opengl_framebuffer( const std::string& base_name, int num_color_attachments, float w, float h )
	: w( w ), h( h ), base_name( base_name )
{
	glCreateFramebuffers( 1, &fb_id );

	bind();

	for( int a = 0 ; a < num_color_attachments ; ++a )
	{
		add_texture();
	}

	// depth/stencil buffer

	glCreateRenderbuffers( 1, &rbo_id );
	glBindRenderbuffer( GL_RENDERBUFFER, rbo_id );
	glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, (int) w, (int) h );
	glBindRenderbuffer( GL_RENDERBUFFER, 0 );

	glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_id );

	if( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
	{
		log_error( "Failed to create frame buffer" );
	}

	// tell opengl to render to all color attachments (by default, it only renders to the first)

	std::vector<unsigned> ids;

	for( unsigned id = 0 ; id < textures.size() ; ++id )
	{
		ids.push_back( GL_COLOR_ATTACHMENT0 + id );
	}

	glDrawBuffers( static_cast<int>( textures.size() ), ids.data() );

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

	if( rbo_id )
	{
		glDeleteRenderbuffers( 1, &rbo_id );
	}

	glBindTexture( GL_TEXTURE_2D, 0 );

	for( auto iter : textures )
	{
		glDeleteTextures( 1, &iter->gl_id );
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

void w_opengl_framebuffer::add_texture()
{
	auto texture_num = static_cast<int>( textures.size() );
	std::string tex_name = fmt::format( "tex{}_{}_frame_buffer", texture_num, base_name );

	auto texture = engine->asset_cache->add( std::make_unique<a_texture>(), tex_name, "" );
	texture->w = w;
	texture->h = h;

	glCreateTextures( GL_TEXTURE_2D, 1, &texture->gl_id );
	glBindTexture( GL_TEXTURE_2D, texture->gl_id );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, (int) w, (int) h, 0, GL_RGBA, GL_FLOAT, nullptr );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + texture_num, GL_TEXTURE_2D, texture->gl_id, 0 );

	texture->subtex = engine->asset_cache->add( std::make_unique<a_subtexture>( tex_name ), "sub_" + tex_name, "" );

	textures.push_back( texture );
}
