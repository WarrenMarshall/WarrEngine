
#include "master_pch.h"
#include "master_header.h"

namespace war
{

file_mem::file_mem( int size )
{
	buffer.resize( size );
}

// ----------------------------------------------------------------------------

file_mem_text::file_mem_text( int size )
{
	buffer.resize( size );
}

void file_mem_text::preprocess()
{
	tokenizer tok( buffer.data(), "\n" );

	size_t count = std::count( buffer.begin(), buffer.end(), '\n' );
	lines.clear();
	lines.reserve( count );

	bool currently_splicing = false;
	std::string spliced_line;

	while( !tok.is_eos() )
	{
		auto next_token = tok.get_next_token();
		auto line = *next_token;

		// skip blank lines
		if( line.length() == 0 )
		{
			continue;
		}

		// skip comment lines
		if( line.substr( 0, 1 ) == "#" )
		{
			continue;
		}

		// strip off trailing comments
		size_t pos = line.find_first_of( '#', 0 );

		if( pos != std::string::npos )
		{
			line = string_util::trim( line.substr( 0, pos ) );
		}

		// handle lines that use continuation characters
		// for ease of reading ('\')

		if( currently_splicing )
		{
			if( line.back() == '\\' )
			{
				spliced_line += std::string( line.substr( 0, line.length() - 1 ) );
			}
			else
			{
				// done splicing the line
				spliced_line += std::string( line );
				currently_splicing = false;
				line = spliced_line;
			}
		}
		else
		{
			if( line.back() == '\\' )
			{
				currently_splicing = true;
				spliced_line = std::string( line.substr( 0, line.length() - 1 ) );
			}
		}

		if( !currently_splicing )
		{
			lines.emplace_back( std::move( line ) );
		}
	}

	buffer.clear();
}

void file_mem_text::preprocess_raw()
{
	tokenizer tok( buffer.data(), "\n" );

	size_t count = std::count( buffer.begin(), buffer.end(), '\n' );
	lines.clear();
	lines.reserve( count );

	while( !tok.is_eos() )
	{
		auto next_token = tok.get_next_token();
		auto line = *next_token;

		// skip blank lines
		if( line.length() == 0 )
		{
			continue;
		}

		lines.emplace_back( std::move( line ) );
	}

	buffer.clear();
}

}
