#pragma once

// ----------------------------------------------------------------------------

struct w_file_disk
{
	FILE* file_handle = nullptr;

	w_file_disk();

	void open_for_read( std::string_view filename );
	void read_glob( void* write_ptr, int size );
	void open_for_write( std::string_view filename );
	void write_glob( void* read_ptr, int size );
	void close();
};
