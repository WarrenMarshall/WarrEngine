#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

w_mem_file::w_mem_file( int size )
{
	buffer = std::make_unique<std::vector<char>>( size );
}
