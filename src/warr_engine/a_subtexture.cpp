
#include "master_pch.h"
#include "master_header.h"

implement_find_func( a_subtexture )

a_subtexture::a_subtexture( const std::string_view tex_name )
{
	tex = a_raw_image_data::find( tex_name );

	rc_tex = { 0, 0, tex->w, tex->h };

	uv00 = w_uv( 0, 0 );
	uv11 = w_uv( 1, 1 );
}

a_subtexture::a_subtexture( const std::string_view texture_tag, const w_rect& rc )
{
	// find the texture being referenced
	tex = a_raw_image_data::find( texture_tag );

	// if W or H are -1, they are defaulted to the textures W/H

	rc_tex = rc;

	rc_tex.w = ( rc.w == -1 ) ? tex->w : rc.w;
	rc_tex.h = ( rc.h == -1 ) ? tex->h : rc.h;

	// note : images are upside down, so the Y coordinate gets flipped across the V axis

	w_rect rc_src(
		rc.x,
		tex->h - rc.y - rc_tex.h,
		rc_tex.w,
		rc_tex.h
	);

	// compute uv coordinates for the top left and bottom right corners. these corners
	// provide enough information to reconstruct UV01 and UV10 when needed.

	uv00 = w_uv(
		rc_src.x / tex->w,
		rc_src.y / tex->h
	);
	uv11 = w_uv(
		( rc_src.x + rc_src.w ) / tex->w,
		( rc_src.y + rc_src.h ) / tex->h
	);
}

w_rect a_subtexture::get_bounding_rect()
{
	return rc_tex;
}
