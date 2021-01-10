
#include "master_pch.h"
#include "master_header.h"

#ifdef USE_THREADED_ASSET_LOADING
static std::mutex cache_mutex;
#endif

// loads a config file from disk and stores it in the cache
void w_cache_asset_definition_files::add( const std::string_view filename )
{
	for( const auto& iter : cache )
	{
		if( iter.original_filename == filename )
		{
			// check the asset_def files as the same resource is being loaded more than once.
			log_error( "Asset definition file already cached : [{}]", filename );
			return;
		}
	}

	w_asset_definition_file asset_def_file = {};
	asset_def_file.original_filename = filename;

	if( asset_def_file.create_internals() )
	{
#ifdef USE_THREADED_ASSET_LOADING
		std::lock_guard<std::mutex> lock( cache_mutex );
#endif
		cache.emplace_back( std::move( asset_def_file ) );
	}
}
