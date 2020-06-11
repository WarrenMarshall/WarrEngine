#pragma once

// ----------------------------------------------------------------------------
// a file that has been read into memory, either from the
// hard drive or a ZIP file

struct w_mem_file
{
	bool was_loaded_from_zip_file = false;

	w_mem_file( int size );

	std::unique_ptr<std::vector<char>> buffer = nullptr;
};
