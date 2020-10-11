#include "master_pch.h"
#include "master_header.h"

w_file_mem::w_file_mem( int size )
{
	buffer = std::make_unique<std::vector<char>>( size );
}

// ----------------------------------------------------------------------------

w_mem_file_text::w_mem_file_text( int size )
	: w_file_mem( size )
{
	lines = std::make_unique<std::vector<std::string>>();
}

void w_mem_file_text::preprocess()
{
	buffer->push_back( '\n' );
	buffer->push_back( '\0' );
	w_tokenizer tok( buffer->data(), '\n' );

	size_t count = std::count( buffer->begin(), buffer->end(), '\n' );
	lines = std::make_unique<std::vector<std::string>>();
	lines->reserve( static_cast<int>( count ) );

	bool currently_splicing = false;
	std::string spliced_line;

	while( !tok.is_eos() )
	{
		auto line = *tok.get_next_token();

		// skip blank lines
		if( line.length() == 0 )
		{
			continue;
		}

		// skip comment lines
		if( line.substr(0,1) == "#" )
		{
			continue;
		}

		// strip off trailing comments
		size_t pos = line.find_first_of( "#", 0 );

		if( pos != std::string::npos )
		{
			line = w_stringutil::trim( line.substr( 0, pos ) );
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
			lines->emplace_back( std::move( line ) );
		}
	}

	buffer = nullptr;
}
