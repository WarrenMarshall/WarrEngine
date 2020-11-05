#pragma once

// ----------------------------------------------------------------------------

struct w_file_disk
{
	FILE* file_handle = nullptr;

	void open_for_read( std::string_view filename );
	bool read_glob( void* write_ptr, int size );
	void open_for_write( std::string_view filename );
	void write_glob( void* read_ptr, int size );
	void close();
};
