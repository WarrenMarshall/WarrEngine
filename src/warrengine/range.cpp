
#include "master_pch.h"

namespace war
{
	Range::Range( f32 start, f32 end ) : start( start ), end( end )
	{

	}

	Range::Range( f32 value ) : start( -value ), end( value )
	{

	}

	Range::Range( std::string_view str )
	{
		Tokenizer tok( str, "," );

		start = Text_Parser::float_from_str( *tok.get_next_token() );
		end = Text_Parser::float_from_str( *tok.get_next_token() );
	}

	f32 Range::clamp_value( f32 value )
	{
		return glm::clamp( value, start, end );
	}

	f32 Range::get_random_value()
	{
		f32 rand = Random::getf_range( 0.f, 1.f );
		return start + ( ( end - start ) * rand );
	}

	f32 Range::get_value_at_pct( f32 pct )
	{
		pct = glm::clamp( pct, 0.f, 1.f );
		return start + ( ( end - start ) * pct );
	}

	f32 Range::get_pct_from_value( f32 value )
	{
		f32 pct = ( value - start ) / ( end - start );

		return glm::clamp( pct, 0.f, 1.f );
	}

	bool Range::is_within( f32 val )
	{
		return ( val >= start and val <= end );
	}

}
