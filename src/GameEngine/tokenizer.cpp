#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

w_tokenizer::w_tokenizer( const std::string& string_buffer, char delim, std::string def_value )
	: string_buffer( string_buffer ), delim( delim ), def_value( def_value ), idx( 0 ), end_of_string( false )
{
}

std::string w_tokenizer::get_next_token()
{
	if( is_eos() )
	{
		return w_stringutil::trim( def_value );
	}

	int start = idx;
	int end = static_cast<int>( string_buffer.find( delim, start ) );

	if( end == std::string::npos )
	{
		// delimiter was not found so set 'end' to consume the rest of the string
		end = static_cast<int>( string_buffer.length() );
		idx = end;
		end_of_string = true;
	}
	else if( start == end )
	{
		idx++;
		return "";
	}

	// extract the token
	std::string ret = string_buffer.substr( start, static_cast<size_t>( end - start ) );

	// move the index to beyond the returning token
	idx += static_cast<int>( ret.length() ) + 1;

	// trim the token before sending it back
	return w_stringutil::trim( ret );
}
