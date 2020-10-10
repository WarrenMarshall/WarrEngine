#pragma once

// a lightweight class that defines a renderable subsection of an a_texture

struct a_subtexture : i_asset
{
	// the texture asset we draw from
	a_texture* tex = nullptr;

	// the width/height of this subtexture
	w_sz sz = w_sz( 0, 0 );

	// the uv coordinates that define this image within the texture
	w_uv uv00 = w_uv( 0, 0 );
	w_uv uv11 = w_uv( 1, 1 );

	a_subtexture( const std::string_view tex_name );
	a_subtexture( const std::string_view tex_name, const w_rect& rc );

	void bind();
	void unbind();
};
