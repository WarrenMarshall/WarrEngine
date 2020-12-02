#pragma once

// a lightweight class that defines a renderable subsection of an a_texture

struct a_subtexture : i_asset
{
	declare_find_func( a_subtexture )

	// the texture asset we draw from
	a_texture* tex = nullptr;

	// the source rectangle on tex that defines this subtexture.
	w_rect rc_tex = { 0,0,0,0 };

	// the uv coordinates that define this image within the texture
	w_uv uv00 = w_uv( 0, 0 );
	w_uv uv11 = w_uv( 1, 1 );

	a_subtexture( const std::string_view texture_tag );
	a_subtexture( const std::string_view texture_tag, const w_rect& rc );

	void bind();
	void unbind();
	w_rect get_bounding_rect();

};