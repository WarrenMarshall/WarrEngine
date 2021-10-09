
namespace war
{

struct Range final
{
	float_t start = 0.f;
	float_t end = 0.f;

	Range() = default;
	Range( float_t start, float_t end )
		: start( start ), end( end )
	{
	}
	Range( float_t value )
		: start( -value ), end( value )
	{
	}

	Range( std::string_view str )
	{
		Tokenizer tok( str, "," );

		start = Text_Parser::float_from_str( *tok.get_next_token() );
		end = Text_Parser::float_from_str( *tok.get_next_token() );
	}

	float_t clamp_value( float_t value )
	{
		return glm::clamp( value, start, end );
	}

	[[nodiscard]] float_t get_random_value()
	{
		float_t rand = Random::getf_range( 0.f, 1.f );
		return start + ( ( end - start ) * rand );
	}

	[[nodiscard]] float_t get_value_at_pct( float_t pct )
	{
		pct = glm::clamp( pct, 0.f, 1.f );
		return start + ( ( end - start ) * pct );
	}

	[[nodiscard]] float_t get_pct_from_value( float_t value )
	{
		float_t pct = ( value - start ) / ( end - start );

		return glm::clamp( pct, 0.f, 1.f );
	}

	[[nodiscard]] bool is_within( float_t val )
	{
		return ( val >= start and val <= end );
	}
};

}
