#include "master_pch.h"
#include "master_header.h"

a_subtexture::a_subtexture( const std::string_view tex_name )
{
	tex = engine->get_asset<a_texture>( tex_name );

	sz.w = tex->w;
	sz.h = tex->h;
}

a_subtexture::a_subtexture( const std::string_view texture_name, const w_rect& rc )
{
	// find the texture being referenced
	tex = engine->get_asset<a_texture>( texture_name );

	// - if W or H are -1, they are defaulted to the textures W/H

	sz.w = ( rc.w == -1 ) ? tex->w : rc.w;
	sz.h = ( rc.h == -1 ) ? tex->h : rc.h;

	// - images are upside down, the Y coordinates get flipped

	w_rect rc_src(
		rc.x,
		tex->h - rc.y - sz.h,
		sz.w,
		sz.h
	);

	uv00 = w_uv(
		rc_src.x / tex->w,
		rc_src.y / tex->h
	);
	uv11 = w_uv(
		( rc_src.x + rc_src.w ) / tex->w,
		( rc_src.y + rc_src.h ) / tex->h
	);
}

void a_subtexture::bind()
{
	tex->bind();
}

void a_subtexture::unbind()
{
	tex->unbind();
}
