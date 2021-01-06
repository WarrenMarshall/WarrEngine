
#include "master_pch.h"
#include "master_header.h"

w_cache_assets::w_cache_assets()
{
	cache.reserve( 200 );
}

a_src_texture* w_cache_assets::find_src_texture_for_tag( const std::string_view tag )
{
	a_src_texture* src_texture = a_src_texture::find( tag, b_silent( true ) );

	if( !src_texture )
	{
		src_texture = a_gradient::find( tag );
	}

	return src_texture;
}
