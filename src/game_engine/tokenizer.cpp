#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

w_tokenizer::w_tokenizer( const std::string_view string_buffer, char delim, std::optional<std::string> def_value )
	: string_buffer( string_buffer ), delim( delim ), def_value( def_value.value_or( "" ) )
{
}

std::optional<std::string_view> w_tokenizer::get_next_token()
{
	if( is_eos() )
	{
		return std::nullopt;
	}

	size_t start = idx;
	size_t end = static_cast<int>( string_buffer.find( delim, start ) );

	if( end == std::string_view::npos )
	{
		// delimiter was not found so set 'end' to consume the rest of the string
		end = string_buffer.length();
		idx = static_cast<int>( end );
		end_of_string = true;
	}
	else if( start == end )
	{
		idx++;
		return std::nullopt;
	}

	// extract the token
	std::string_view ret = string_buffer.substr( start, end - start );

	// move the index to beyond the returning token
	idx += static_cast<int>( ret.length() ) + 1;

	// trim the token before sending it back
	return w_stringutil::trim( ret );
}

bool w_tokenizer::is_eos()
{
	return end_of_string;
}
