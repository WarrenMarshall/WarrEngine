
namespace war
{

// a generalized way to cache a value for a key. can be used to speed up
// anything that does consecutive look ups that might be expensive.

template<typename T_Key, typename T_Value>
struct Cache final
{
	// it's tempting to change this to std::unordered_map but that will cause
	// serious perf issues as the key value will need to be hashed internally.
	// if you instead use this class with a key that is already unique (like a
	// pointer) you can use std::map and get max speed out of it.
	std::map<T_Key, T_Value> values;

	// a sanity check value. the cache will asset out if more than this many
	// items is added into it. the logic being that if the cache is huge, is
	// there a compelling reason to be using a cache in that situation?
	i32 cache_size_limit = 10;

	void set( T_Key key, T_Value value )
	{
		values.insert_or_assign( key, value );

		// if this fires, review the called code and make sure caching is worth
		// it. with this many look ups, you might not be saving any perf.
		assert( values.size() < cache_size_limit );
	}

	void clear()
	{
		values.clear();
	}

	[[nodiscard]] std::optional<T_Value> get( T_Key key ) const
	{
		auto iter = values.find( key );

		if( iter == values.end() )
		{
			return std::nullopt;
		}

		return iter->second;
	}
};

}
