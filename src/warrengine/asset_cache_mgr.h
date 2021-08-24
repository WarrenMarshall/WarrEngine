
namespace war
{

struct asset_cache_mgr final
{
	std::unordered_map<std::string, std::unique_ptr<asset>> cache;

	template<typename T>
	T* add( std::unique_ptr<T> asset, std::string_view tag, std::string_view filename )
	{
		auto full_tag = generate_full_asset_tag<T>( tag );

		// the asset is already cached - why are we trying to cache it again?
		assert( cache.find( full_tag ) == cache.end() );

		// save it into the cache and return a raw pointer to the caller

		asset->tag = full_tag;
		asset->original_filename = filename;

		T* return_ptr = asset.get();
		cache[ full_tag ] = std::move( asset );
		return return_ptr;
	}

	template<typename T>
	[[nodiscard]] std::string generate_full_asset_tag( std::string_view tag )
	{
		// if the tag being searched for is NOT already decorated with the
		// type name, add the type name as a prefix.

		if( tag.find_first_of( ':', 0 ) == std::string::npos )
		{
			// this is compiler dependent, but since MSVC returns an unmangled name for
			// the type - I'm using it. MSVC will return type names like "struct name"
			// so we just have to strip off the "struct " prefix and we're good to go.
			std::string_view tag_prefix = typeid( T ).name();
			return std::format( "{}::{}", tag_prefix.substr( 7, std::string::npos ), tag );
		}

		return tag.data();
	}

	template<typename T>
	[[nodiscard]] T* find( std::string_view tag )
	{
		auto full_tag = generate_full_asset_tag<T>( tag );
		auto iter = cache.find( full_tag );

		// if the asset isn't in the cache, that's fatal.
		// check the asset_def files and make sure it's been requested.

		if( iter == cache.end() )
		{
			log_fatal( "not found : [{}]", full_tag );
		}

		return static_cast<T*>( iter->second.get() );
	}

	template<typename T>
	[[nodiscard]] T* find_safe( std::string_view tag )
	{
		auto full_tag = generate_full_asset_tag<T>( tag );
		auto iter = cache.find( full_tag );

		// if the asset isn't in the cache, that's fatal.
		// check the asset_def files and make sure it's been requested.

		if( iter == cache.end() )
		{
			return nullptr;
		}

		return static_cast<T*>( iter->second.get() );
	}
};

}
