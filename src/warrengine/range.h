
namespace war
{

struct Range final
{
	f32 start = 0.f;
	f32 end = 0.f;

	Range() = default;
	Range( f32 start, f32 end )
		: start( start ), end( end )
	{
	}
	Range( f32 value )
		: start( -value ), end( value )
	{
	}

	Range( std::string_view str )
	{
		Tokenizer tok( str, "," );

		start = Text_Parser::float_from_str( *tok.get_next_token() );
		end = Text_Parser::float_from_str( *tok.get_next_token() );
	}

	f32 clamp_value( f32 value )
	{
		return glm::clamp( value, start, end );
	}

	[[nodiscard]] f32 get_random_value()
	{
		f32 rand = Random::getf_range( 0.f, 1.f );
		return start + ( ( end - start ) * rand );
	}

	[[nodiscard]] f32 get_value_at_pct( f32 pct )
	{
		pct = glm::clamp( pct, 0.f, 1.f );
		return start + ( ( end - start ) * pct );
	}

	[[nodiscard]] f32 get_pct_from_value( f32 value )
	{
		f32 pct = ( value - start ) / ( end - start );

		return glm::clamp( pct, 0.f, 1.f );
	}

	[[nodiscard]] bool is_within( f32 val )
	{
		return ( val >= start and val <= end );
	}
};

}
