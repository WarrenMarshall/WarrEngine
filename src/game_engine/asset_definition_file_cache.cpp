
#include "master_pch.h"
#include "master_header.h"

// loads a config file from disk and stores it in the cache
void w_cache_asset_definition_files::add( const std::string_view filename )
{
	for( const auto& iter : cache )
	{
		if( iter->original_filename == filename )
		{
			log_warning( fmt::format( "{} : asset definition file already cached : [{}]", __FUNCTION__, filename ) );
			return;
		}
	}
	auto asset_def_file = std::make_unique<w_asset_definition_file>();

	asset_def_file->original_filename = filename;
	if( asset_def_file->create_internals( b_is_hot_reloading( false ) ) )
	{
		// save it into the cache

		if( g_allow_hot_reload )
		{
			engine->hot_reloadables.emplace_back( asset_def_file.get() );
		}

		cache.emplace_back( std::move( asset_def_file ) );
	}
}
