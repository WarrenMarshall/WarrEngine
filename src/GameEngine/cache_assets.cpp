
#include "master_pch.h"
#include "master_header.h"

i_asset* w_cache_assets::add( std::shared_ptr<i_asset> asset, const std::string_view name, const std::string_view filename )
{
	auto iter = cache.find( std::string( name ) );

	if( iter != cache.end() )
	{
		log_warning( fmt::format( "{} : asset '{}' already cached", __FUNCTION__, name ) );
		return nullptr;
	}

	asset->name = name;
	asset->original_filename = filename;

	if( g_allow_hot_reload )
	{
		if( !asset->original_filename.empty() )
		{
			engine->hot_reloadables.emplace_back( static_cast<i_reloadable*>( asset.get() ) );
		}
	}

	// save it into the cache
	cache.insert( std::make_pair( name, std::move( asset ) ) );

	return find( name, b_silent(true) );
}

i_asset* w_cache_assets::find( const std::string_view name, bool silent )
{
	auto iter = cache.find( std::string( name ) );

	if( iter == cache.end() )
	{
		if( !silent )
			log_error( fmt::format( "{} : not found : [{}]", __FUNCTION__, name ) );

		return nullptr;
	}

	return iter->second.get();
}
