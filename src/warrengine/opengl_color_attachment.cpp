
#include "master_pch.h"
#include "master_header.h"

w_opengl_color_attachment::w_opengl_color_attachment( w_opengl_framebuffer* fb_owner, const w_color& clear_color )
	: fb_owner( fb_owner ), clear_color( clear_color )
{
	// figure out how to identify the internal texture
	auto texture_num = (int)fb_owner->color_attachments.size();
	std::string tex_name = fmt::format( "tex{}_{}_frame_buffer", texture_num, fb_owner->base_name );

	// create the internal texture
	engine->asset_cache->add( std::make_unique<a_src_texture>(), tex_name, "" );
	texture = engine->asset_cache->add( std::make_unique<a_texture>( tex_name ), "fb_" + tex_name, "" );
	texture->src_texture->w = viewport_w;
	texture->src_texture->h = viewport_h;

	// create the opengl texture
	glCreateTextures( GL_TEXTURE_2D, 1, &texture->src_texture->gl_id );
	glBindTextureUnit( 0, texture->src_texture->gl_id );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA16F, (int)viewport_w, (int)viewport_h, 0, GL_RGBA, GL_FLOAT, nullptr );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	// attach to the owning frame buffer
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + texture_num, GL_TEXTURE_2D, texture->src_texture->gl_id, 0 );
}

void w_opengl_color_attachment::clear()
{
	glClearTexImage( texture->src_texture->gl_id, 0, GL_RGBA, GL_FLOAT, &( clear_color.r ) );
}