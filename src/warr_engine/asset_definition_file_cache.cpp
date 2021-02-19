
#include "master_pch.h"
#include "master_header.h"

// loads a config file from disk and stores it in the cache
void w_cache_asset_definition_files::add( const std::string_view filename )
{
	w_asset_definition_file asset_def_file = {};
	asset_def_file.original_filename = filename;

	if( asset_def_file.create_internals() )
	{
		cache.emplace_back( std::move( asset_def_file ) );
	}
}
