#pragma once

// defines a subsection of an a_src_texture

struct a_texture : i_asset
{
	declare_find_func( a_texture )

	// the raw_image asset this subtexture sources from
	a_src_texture* src_texture = nullptr;

	// the rectangle on the raw_image that defines this subtexture
	w_rect rc_tex = { 0,0,0,0 };

	// the uv coordinates that define this image within the raw_image
	w_uv uv00 = w_uv( 0, 0 );
	w_uv uv11 = w_uv( 1, 1 );

	a_texture( const std::string_view src_texture_tag );
	a_texture( const std::string_view src_texture_tag, const w_rect& rc );

	w_rect get_bounding_rect();

};
