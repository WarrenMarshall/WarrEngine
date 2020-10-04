#include "master_pch.h"
#include "master_header.h"

a_subtexture::a_subtexture( const std::string_view tex_name )
{
	tex = engine->get_asset<a_texture>( tex_name );

	rc_src.w = sz.w = tex->w;
	rc_src.h = sz.h = tex->h;
}

a_subtexture::a_subtexture( const std::string_view texture_name, const w_rect& rc )
{
	// find the texture being referenced
	tex = engine->get_asset<a_texture>( texture_name );

	rc_src = rc;

	if( rc.w == -1 ) { rc_src.w = tex->w; }
	if( rc.h == -1 ) { rc_src.h = tex->h; }

	rc_src.y = tex->h - rc.y - rc_src.h;

	sz.w = rc_src.w;
	sz.h = rc_src.h;

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
