
#include "master_pch.h"
#include "master_header.h"

// loads a config file from disk and stores it in the cache
w_asset_definition_file* w_cache_asset_definition_files::add( const char* name, const char* filename )
{
	auto iter = cache.find( name );

	if( iter != cache.end() )
	{
		log_warning( "%s : asset definition file already cached : [%s]", __FUNCTION__, filename );
		return nullptr;
	}

	auto cfg = std::make_unique<w_asset_definition_file>();

	cfg->name = name;
	cfg->original_filename = filename;
	if( cfg->create_internals( false ) )
	{
		// save it into the cache
		//log_msg( "%s : caching : [%s]", __FUNCTION__, filename );

		if( g_allow_hot_reload )
		{
			engine->hot_reloadables.push_back( cfg.get() );
		}

		cache.insert( std::make_pair( std::string( name ), std::move( cfg ) ) );
	}

	return cfg.get();
}

w_asset_definition_file* w_cache_asset_definition_files::find( const char* name, bool silent )
{
	auto iter = cache.find( name );

	if( iter == cache.end() )
	{
		if( !silent )
		{
			log_error( "%s : config not cached : [%s]", __FUNCTION__, name );
		}
		return nullptr;
	}

	return iter->second.get();
}
