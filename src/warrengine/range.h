
namespace war
{

template<typename T>
struct range
{
	T start = {};
	T end = {};

	range() = delete;
	range( T start, T end )
		: start( start ), end( end )
	{
	}
	range( T value )
		: start( -value ), end( value )
	{
	}

	range( std::string_view str )
	{
		tokenizer tok( str, "," );

		start = text_parser::float_from_str( *tok.get_next_token() );
		end = text_parser::float_from_str( *tok.get_next_token() );
	}

	float clamp_value( T value )
	{
		return glm::clamp( value, start, end );
	}

	[[nodiscard]] T get_random_value()
	{
		float rand = random::getf_range( 0.f, 1.f );
		return start + ( ( end - start ) * rand );
	}

	[[nodiscard]] T get_value_at_pct( float pct )
	{
		pct = glm::clamp( pct, 0.f, 1.f );
		return start + ( ( end - start ) * pct );
	}

	[[nodiscard]] T get_pct_from_value( T value )
	{
		float pct = ( value - start ) / ( end - start );

		return glm::clamp( pct, 0.f, 1.f );
	}

	[[nodiscard]] bool is_within( T val )
	{
		return ( val >= start and val <= end );
	}
};

}
