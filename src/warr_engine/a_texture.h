#pragma once

// a texture is a subsection of an a_src_texture asset. it can
// represent the entire source texture or a sub rectangle of it.
//
// these are what gets passed to the renderer functions.

struct a_texture : i_asset
{
	declare_find_func( a_texture )

	// the raw_image asset this texture sources from
	a_src_texture* src_texture = nullptr;

	// the rectangle on the raw_image that defines this texture
	w_rect rc = {};

	// the uv coordinates that define this image within the raw_image
	w_uv uv00 = w_uv( 0, 0 );
	w_uv uv11 = w_uv( 1, 1 );

	a_texture( const std::string_view src_texture_tag );
	a_texture( const std::string_view src_texture_tag, const w_rect& rc_tex );
};
