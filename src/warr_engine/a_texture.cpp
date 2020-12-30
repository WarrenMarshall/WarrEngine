
#include "master_pch.h"
#include "master_header.h"

implement_find_func( a_texture )

a_texture::a_texture( const std::string_view src_texture_tag )
{
	src_texture = a_src_texture::find( src_texture_tag );

	rc = { 0, 0, src_texture->w, src_texture->h };

	uv00 = w_uv( 0, 0 );
	uv11 = w_uv( 1, 1 );
}

a_texture::a_texture( const std::string_view src_texture_tag, const w_rect& rc_tex )
{
	// find the texture being referenced
	src_texture = a_src_texture::find( src_texture_tag );

	// if W or H are -1, they are defaulted to the textures W/H

	rc = rc_tex;

	rc.w = ( rc_tex.w == -1 ) ? src_texture->w : rc_tex.w;
	rc.h = ( rc_tex.h == -1 ) ? src_texture->h : rc_tex.h;

	// note : images are upside down, so the Y coordinate gets flipped across the V axis

	w_rect rc_src(
		rc_tex.x,
		src_texture->h - rc_tex.y - rc_tex.h,
		rc_tex.w,
		rc_tex.h
	);

	// compute uv coordinates for the top left and bottom right corners. these corners
	// provide enough information to reconstruct UV01 and UV10 when needed.

	uv00 = w_uv(
		rc_src.x / src_texture->w,
		rc_src.y / src_texture->h
	);
	uv11 = w_uv(
		( rc_src.x + rc_src.w ) / src_texture->w,
		( rc_src.y + rc_src.h ) / src_texture->h
	);
}
