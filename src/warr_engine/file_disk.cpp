
#include "master_pch.h"
#include "master_header.h"

void w_file_disk::open_for_read( std::string_view filename )
{
	fopen_s( &file_handle, filename.data(), "rb" );

	if( file_handle == nullptr )
	{
		log_error( "Unable to open file '{}'", filename );
	}
}

bool w_file_disk::read_glob( void* write_ptr, size_t size )
{
	size_t elements_read = fread_s( write_ptr, size, 1, size, file_handle );

	if( elements_read == 0 || elements_read != size )
	{
		log_warning( "Read error on glob (expected to read {} elements, got {})", size, elements_read );
		return false;
	}

	return true;
}

void w_file_disk::open_for_write( std::string_view filename )
{
	engine->fs->create_path_if_not_exist( filename );

	fopen_s( &file_handle, filename.data(), "wb" );

	if( file_handle == nullptr )
	{
		log_error( "Unable to open file '{}'", filename );
	}
}

void w_file_disk::write_glob( void* read_ptr, size_t size )
{
	size_t elements_written = fwrite( read_ptr, 1, size, file_handle );

	if( elements_written == 0 || elements_written != size )
	{
		log_error( "Write error on glob (expected to write {} elements, wrote {})", size, elements_written );
	}
}

void w_file_disk::close()
{
	fclose( file_handle );
}
