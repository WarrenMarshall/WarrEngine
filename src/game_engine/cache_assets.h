#pragma once

struct w_cache_assets
{
	std::unordered_map<std::string, std::unique_ptr<i_asset>> cache;

	template<typename T>
	T* add( std::unique_ptr<T> asset, const std::string_view tag, const std::string_view filename )
	{
		auto iter = cache.find( std::string( tag ) );

		if( iter != cache.end() )
		{
			log_msg( "Asset '{}' already cached", tag );
			return nullptr;
		}

		asset->tag = tag;
		asset->original_filename = filename;

		// save it into the cache
		cache.insert( std::make_pair( tag, std::move( asset ) ) );

		return find<T>( tag, b_silent( true ) );
	}

	template<typename T>
	_NODISCARD T* find( const std::string_view tag, bool silent )
	{
		auto iter = cache.find( std::string( tag ) );

		// if the asset isn't in the cache, that's fatal.
		// check the asset_def files and make sure it's been requested.

		if( iter == cache.end() )
		{
			if( !silent )
				log_error( "not found : [{}]", tag );

			return nullptr;
		}

		T* asset_ptr = static_cast<T*>( iter->second.get() );

#ifdef _DEBUG
		// if we found an asset but it's the wrong type, that's also fatal - name things uniquely!
		if( dynamic_cast<T*>( asset_ptr ) == nullptr )
		{
			log_msg( "Asset WAS found but the type doesn't match the requested type" );
			log_msg( "	[{}]", tag );
			log_msg( "	Requested type : \"{}\"", typeid( T ).name() );
			log_msg( "	Type in cache  : \"{}\"", typeid( *asset_ptr ).name() );
			assert( false );
		}
#endif

		return static_cast<T*>( asset_ptr );
	}
};
