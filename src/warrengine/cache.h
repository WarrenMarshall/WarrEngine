
namespace war
{

// a generalized way to cache a value for a key. can be used to speed up
// anything that does consecutive look ups that might be expensive.

template<typename T_Key, typename T_Value>
struct Cache final
{
	std::map<T_Key, T_Value> values;
	i32 cache_size_limit = 10;

	void set( T_Key key, T_Value value )
	{
		values.insert_or_assign( key, value );

		// if this fires, review the called code and make sure the caching is
		// worth it. with this many look ups, it might not be saving anything.
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
