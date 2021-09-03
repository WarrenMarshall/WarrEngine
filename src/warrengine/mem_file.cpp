
#include "master_pch.h"
#include "master_header.h"

namespace war
{

Mem_File::Mem_File( int32_t size )
{
	buffer.resize( size );
}

// ----------------------------------------------------------------------------

Mem_File_Text::Mem_File_Text( int32_t size )
{
	buffer.resize( size );
}

void Mem_File_Text::preprocess()
{
	Tokenizer tok( buffer.data(), "\n" );

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
			line = String_Util::trim( line.substr( 0, pos ) );
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
			lines.emplace_back( line );
		}
	}

	buffer.clear();
}

void Mem_File_Text::preprocess_raw()
{
	Tokenizer tok( buffer.data(), "\n" );

	size_t count = std::count( buffer.begin(), buffer.end(), '\n' );
	lines.clear();
	lines.reserve( count );

	while( !tok.is_eos() )
	{
		auto next_token = tok.get_next_token();
		auto& line = *next_token;

		// skip blank lines
		if( line.length() == 0 )
		{
			continue;
		}

		lines.emplace_back( line );
	}

	buffer.clear();
}

}
