
namespace war
{

template<typename T>
struct Range final
{
	T start = {};
	T end = {};

	Range() = delete;
	Range( T start, T end )
		: start( start ), end( end )
	{
	}
	Range( T value )
		: start( -value ), end( value )
	{
	}

	Range( std::string_view str )
	{
		Tokenizer tok( str, "," );

		start = Text_Parser::float_from_str( *tok.get_next_token() );
		end = Text_Parser::float_from_str( *tok.get_next_token() );
	}

	float_t clamp_value( T value )
	{
		return glm::clamp( value, start, end );
	}

	[[nodiscard]] T get_random_value()
	{
		float_t rand = Random::getf_range( 0.f, 1.f );
		return start + ( ( end - start ) * rand );
	}

	[[nodiscard]] T get_value_at_pct( float_t pct )
	{
		pct = glm::clamp( pct, 0.f, 1.f );
		return start + ( ( end - start ) * pct );
	}

	[[nodiscard]] T get_pct_from_value( T value )
	{
		float_t pct = ( value - start ) / ( end - start );

		return glm::clamp( pct, 0.f, 1.f );
	}

	[[nodiscard]] bool is_within( T val )
	{
		return ( val >= start and val <= end );
	}
};

}
