
#include "master_pch.h"
#include "master_header.h"

// loads a config file from disk and stores it in the cache
w_asset_definition_file* w_cache_asset_definition_files::add( const std::string& name, const std::string& filename )
{
	const auto iter = cache.find( name );

	if( iter != cache.end() )
	{
		log_warning( "%s : asset definition file already cached : [%s]", __FUNCTION__, filename.c_str() );
		return nullptr;
	}

	auto cfg = std::make_unique<w_asset_definition_file>();

	cfg->name = name;
	cfg->original_filename = filename;
	if( cfg->create_internals( false ) )
	{
		// save it into the cache

		if( g_allow_hot_reload )
		{
			engine->hot_reloadables.emplace_back( cfg.get() );
		}

		cache.insert( std::make_pair( name, std::move( cfg ) ) );
	}

	return cache.find( name )->second.get();
}
