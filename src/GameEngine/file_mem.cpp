#include "master_pch.h"
#include "master_header.h"

w_mem_file::w_mem_file()
{
	buffer = std::make_unique<std::vector<char>>();
}

// ----------------------------------------------------------------------------

w_mem_file_text::w_mem_file_text()
{
	lines = std::make_unique<std::vector<std::string>>();
}

void w_mem_file_text::preprocess()
{
	buffer->push_back( '\n' );
	buffer->push_back( '\0' );
	w_tokenizer tok( buffer->data(), '\n', "" );

	size_t count = std::count( buffer->begin(), buffer->end(), '\n' );
	lines = std::make_unique<std::vector<std::string>>();
	lines->reserve( static_cast<int>( count ) );

	while( !tok.is_eos() )
	{
		auto line = tok.get_next_token();

		// skip blank lines
		if( line.length() == 0 )
		{
			continue;
		}

		lines->emplace_back( std::move( line ) );
	}

	buffer = nullptr;
}
