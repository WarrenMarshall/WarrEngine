
#include "master_pch.h"
#include "master_header.h"

namespace war
{

tokenizer::tokenizer( std::string_view string_buffer, std::string_view delim )
{
	init( string_buffer, delim );
}

void tokenizer::init( std::string_view string_buffer, std::string_view delim )
{
	this->string_buffer = string_buffer;
	this->delim = delim;

	preprocess();
	idx = 0;
}

// breaks up the original string buffer into a set of delimited
// string views for easy access later.
//
// NOTE : text inside of a bracket set "[]" is kept whole as a single token.

void tokenizer::preprocess()
{
	size_t start = 0, end = 0;
	auto bracket_depth = 0;

	tokens.clear();

	for( auto ch : string_buffer )
	{
		auto pos = delim.find_first_of( ch );

		if( pos != std::string::npos and bracket_depth == 0 )
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

	if( end > start )
	{
		tokens.emplace_back( string_buffer.substr( start, end - start ) );
	}
}

std::optional<std::string_view> tokenizer::get_next_token()
{
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
	return string_util::trim( *ret );
}

bool tokenizer::is_eos()
{
	return idx >= tokens.size();
}

}
