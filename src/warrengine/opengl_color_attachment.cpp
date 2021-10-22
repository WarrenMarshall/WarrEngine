
#include "master_pch.h"
#include "master_header.h"

namespace war
{

OpenGL_Color_Attachment::OpenGL_Color_Attachment( OpenGL_Frame_Buffer* fb_owner, Vec2 sz, const Color& clear_color )
	: fb_owner( fb_owner ), clear_color( clear_color )
{

	// figure out how to identify the internal texture
	auto texture_num = (i32)fb_owner->color_attachments.size();
	std::string tex_name( std::format( "tex_{}_{}_fb", texture_num, fb_owner->base_name ) );

	// create the internal texture
	g_engine->asset_cache.add( std::make_unique<Texture_Source_Asset>(), tex_name, "" );
	texture = g_engine->asset_cache.add( std::make_unique<Texture_Asset>( tex_name ), "fb_" + tex_name, "" );
	auto src_texture = texture->get_src_texture();

	src_texture->w = sz.w;
	src_texture->h = sz.h;

	// create the opengl texture
	glCreateTextures( GL_TEXTURE_2D, 1, &src_texture->gl_id );
	glBindTextureUnit( 0, src_texture->gl_id );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB16F, (i32)sz.w, (i32)sz.h, 0, GL_RGB, GL_FLOAT, nullptr );

	glTextureParameteri( src_texture->gl_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTextureParameteri( src_texture->gl_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTextureParameteri( src_texture->gl_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTextureParameteri( src_texture->gl_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	// attach to the owning frame buffer
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + texture_num, GL_TEXTURE_2D, texture->get_src_texture()->gl_id, 0 );
}

void OpenGL_Color_Attachment::clear()
{
	glClearTexImage( texture->get_src_texture()->gl_id, 0, GL_RGB, GL_FLOAT, &( clear_color.r ) );
}

}
