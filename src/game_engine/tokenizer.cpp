#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

w_tokenizer::w_tokenizer( std::string_view string_buffer, char delim )
	: string_buffer( string_buffer ), delim( delim )
{
	preprocess();
	idx = 0;
}

// breaks up the original string buffer into a set of delimited
// string views for easy access later.
//
// NOTE : text inside of a bracket set "[]" is kept whole as a single token.

void w_tokenizer::preprocess()
{
	size_t start = 0, end = 0;
	auto bracket_depth = 0;

	tokens.clear();

	for( auto ch : string_buffer )
	{
		if( ch == delim && bracket_depth == 0 )
		{
			if( end > start )
			{
				tokens.emplace_back( string_buffer.substr( start, end - start ) );
			}

			start = end + 1;
		}
		else if( ch == '[' )
		{
			bracket_depth++;
		}
		else if( ch == ']' )
		{
			bracket_depth--;
		}

		end++;
	}

	// mismatched [] bracket set in string buffer
	//assert( bracket_depth == 0 );

	if( end > start )
	{
		tokens.emplace_back( string_buffer.substr( start, end - start ) );
	}
}

std::optional<std::string_view> w_tokenizer::get_next_token()
{
#if 1
	if( is_eos() )
	{
		return std::nullopt;
	}

	if( idx >= tokens.size() )
	{
		idx = tokens.size();
		end_of_string = true;
		return std::nullopt;
	}

	std::optional<std::string_view> ret = std::nullopt;

	// extract the token
	ret = tokens[ idx ];

	// move the index to the next token
	idx++;

	// trim the token before sending it back
	return w_string_util::trim( *ret );
#else
	if( is_eos() )
	{
		return std::nullopt;
	}

	size_t start = idx;
	size_t end = string_buffer.find( delim, start );

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

	std::optional<std::string_view> ret = std::nullopt;

	// extract the token
	ret = string_buffer.substr( start, end - start );

	// move the index to beyond the returning token
	idx += ret->length() + 1;

	// trim the token before sending it back
	return w_string_util::trim( *ret );
#endif
}

bool w_tokenizer::is_eos()
{
	return idx >= tokens.size();
}
