
#include "master_pch.h"
#include "master_header.h"

i_asset* w_cache_assets::add( std::unique_ptr<i_asset> asset, const std::string& name, const std::string& filename )
{
	auto iter = cache.find( name );

	if( iter != cache.end() )
	{
		log_warning( "%s : asset '%s' already cached : [%s]", __FUNCTION__, name, filename.c_str() );
		return nullptr;
	}

	asset->name = name;
	asset->original_filename = filename;

	if( g_allow_hot_reload )
		engine->hot_reloadables.push_back( static_cast<i_reloadable*>( asset.get() ) );

	// save it into the cache
	log_msg( "%s : '%s' cached", __FUNCTION__, name.c_str() );
	cache.insert( std::make_pair( name, std::move( asset ) ) );

	return find( name, true );
}

i_asset* w_cache_assets::find( const std::string& name, bool silent )
{
	auto iter = cache.find( name );

	if( iter == cache.end() )
	{
		if( !silent )
			log_error( "%s : not found : [%s]", __FUNCTION__, name.c_str() );

		return nullptr;
	}

	return iter->second.get();
}
