
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
	auto cfg = std::make_unique<w_asset_definition_file>();

	cfg->original_filename = filename;
	if( cfg->create_internals( b_is_hot_reloading( false ) ) )
	{
		// save it into the cache

		if( g_allow_hot_reload )
		{
			engine->hot_reloadables.emplace_back( cfg.get() );
		}

		cache.emplace_back( std::move( cfg ) );
	}
}
