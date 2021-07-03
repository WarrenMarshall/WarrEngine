
#include "master_pch.h"
#include "master_header.h"

namespace war
{

bool tile_set_asset::create()
{
	// create the individual tiles from the tile set info

	auto tile_count = (size_t)( ( texture->get_src_texture()->w / tile_sz ) * ( texture->get_src_texture()->h / tile_sz ) );
	tiles.reserve( tile_count );

	auto w = texture->get_src_texture()->w / tile_sz;
	auto h = texture->get_src_texture()->h / tile_sz;

	for( auto y = 0 ; y < h ; ++y )
	{
		for( auto x = 0 ; x < w ; ++x )
		{
			tiles.emplace_back(
				texture->get_src_texture()->tag,
				rect( x * tile_sz, y * tile_sz, tile_sz, tile_sz )
			);
		}
	}

	return true;
}

}
