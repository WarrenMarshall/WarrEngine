#pragma once

struct w_cache_assets
{
	std::unordered_map<std::string, std::unique_ptr<i_asset>> cache;

	w_cache_assets();

	template<typename T>
	T* add( std::unique_ptr<T> asset, const std::string& tag, const std::string_view filename )
	{
		// this is compiler dependent, but since MSVC returns an unmangled name for
		// the type - I'm using it. MSVC will return type names like "struct name"
		// so we just have to strip off the "struct " prefix and we're good to go.
		std::string tag_prefix = typeid( T ).name();
		std::string full_tag = fmt::format( "{}::{}", tag_prefix.substr( 7, std::string::npos ), tag );

		auto iter = cache.find( full_tag );

		if( iter != cache.end() )
		{
			log_error( "Asset '{}' already cached", full_tag );
			return nullptr;
		}

		asset->tag = full_tag;
		asset->original_filename = filename;

		// save it into the cache and return a raw pointer to the caller

		T* return_ptr = asset.get();
		cache.insert( std::make_pair( full_tag, std::move( asset ) ) );
		return return_ptr;
	}

	_NODISCARD a_src_texture* find_src_texture_for_tag( const std::string_view tag );

	template<typename T>
	_NODISCARD T* find( const std::string_view tag, bool silent )
	{
		std::string full_tag = std::string( tag );

		// if the tag being searched for is NOT already decorated with the
		// type name, add the type name as a prefix.
		if( tag.find_first_of( ':', 0 ) == std::string::npos )
		{
			// this is compiler dependent, but since MSVC returns an unmangled name for
			// the type - I'm using it. MSVC will return type names like "struct name"
			// so we just have to strip off the "struct " prefix and we're good to go.
			std::string tag_prefix = typeid( T ).name();
			full_tag = fmt::format( "{}::{}", tag_prefix.substr( 7, std::string::npos ), tag );
		}

		auto iter = cache.find( full_tag );

		// if the asset isn't in the cache, that's fatal.
		// check the asset_def files and make sure it's been requested.

		if( iter == cache.end() )
		{
			if( !silent )
			{
				log_error( "not found : [{}]", full_tag );
			}

			return nullptr;
		}

		return static_cast<T*>( iter->second.get() );
	}
};
