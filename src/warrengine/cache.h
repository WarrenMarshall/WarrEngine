
namespace war
{

// a generalized way to cache a value for a key. can be used to speed up
// anything that does consecutive look ups that might be expensive.

template<typename T_Key, typename T_Value>
struct cache final
{
	T_Key key;
	T_Value value;

	void set( T_Key key, T_Value value )
	{
		this->key = key;
		this->value = value;
	}

	void clear()
	{
		key = {};
	}

	[[nodiscard]] std::optional<T_Value> get( T_Key key ) const
	{
		if( this->key == key )
		{
			return value;
		}

		return std::nullopt;
	}
};

}
