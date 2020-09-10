#pragma once

struct w_cache_assets
{
	std::map<std::string, std::unique_ptr<i_asset>> cache;

	template<typename T>
	T* add( std::unique_ptr<T> asset, const std::string_view name, const std::string_view filename )
	{
		auto iter = cache.find( std::string( name ) );

		if( iter != cache.end() )
		{
			logfile->msg( fmt::format( "{} : asset '{}' already cached", __FUNCTION__, name ) );
			return nullptr;
		}

		asset->name = name;
		asset->original_filename = filename;

		// save it into the cache
		cache.insert( std::make_pair( name, std::move( asset ) ) );

		return find<T>( name, b_silent( true ) );
	}

	template<typename T>
	T* find( const std::string_view name, bool silent )
	{
		auto iter = cache.find( std::string( name ) );

		// if the asset isn't in the cache, that's fatal.
		// check the asset_def files and make sure it's been requested.

		if( iter == cache.end() )
		{
			if( !silent )
				log_error( fmt::format( "{} : not found : [{}]", __FUNCTION__, name ) );

			return nullptr;
		}

		T* asset_ptr = static_cast<T*>( iter->second.get() );

#ifdef _DEBUG
		// if we found an asset but it's the wrong type, that's also fatal - name things uniquely!
		if( dynamic_cast<T*>( asset_ptr ) == nullptr )
		{
			log_msg( fmt::format( "{} : asset WAS found but the type doesn't match the requested type", __FUNCTION__ ) );
			log_msg( fmt::format( "	[{}]", name ) );
			log_msg( fmt::format( "	Requested type : \"{}\"", typeid( T ).name() ) );
			log_msg( fmt::format( "	Type in cache  : \"{}\"", typeid( *asset_ptr ).name() ) );
			assert( false );
		}
#endif

		return static_cast<T*>( asset_ptr );
	}
};
