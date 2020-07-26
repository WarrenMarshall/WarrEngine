#include "master_pch.h"
#include "master_header.h"

w_zip_toc_entry::w_zip_toc_entry( std::string zip_filename, std::string filename, int offset, int size )
	: zip_filename( zip_filename ), filename( filename ), offset_from_start_of_file( offset ), size( size )
{
}

// ----------------------------------------------------------------------------

void w_io_zip::scan_and_build_table_of_contents()
{
	table_of_contents.clear();

	std::string ext, zip_filename;

	// iterate all the files in the active folder
	for( auto& iter : std::filesystem::directory_iterator( "." ) )
	{
		zip_filename = iter.path().generic_string();
		ext = iter.path().filename().extension().generic_string();

		// for each zip file we find...
		if( ext == ".zip" )
		{
			// read the zip file into memory for quick parsing via pointer arithmetic

			std::ifstream file( zip_filename.c_str(), std::ios::binary | std::ios::ate );
			std::streamsize size = file.tellg();
			file.seekg( 0, std::ios::beg );

			std::vector<char> buffer( size );
			if( file.read( buffer.data(), size ) )
			{
				char filename[FILENAME_MAX];
				const char* rptr = buffer.data();

				while( true )
				{
					// local file header
					if( *( (int*) rptr ) == 0x04034b50 )
					{
						auto hdr = (zip_local_file_header*) rptr;
						rptr += sizeof( zip_local_file_header );

						if( hdr->version_needed_to_extract == 10 )
						{
							memset( &filename[0], 0, FILENAME_MAX );
							strncpy_s( &filename[0], FILENAME_MAX, rptr, (size_t) hdr->file_name_length );

							rptr += hdr->file_name_length;

							table_of_contents.insert(
								std::make_pair(
									filename,
									std::move( std::make_unique<w_zip_toc_entry>( zip_filename, filename, (int) ( rptr - buffer.data() ), hdr->uncompressed_size ) )
								)
							);
							rptr += hdr->uncompressed_size;
						}
						else if( hdr->version_needed_to_extract == 20 )
						{
							// FOLDER - skip over it
							rptr += hdr->file_name_length;
						}
						else
						{
							// Anything else means the entry is compressed and/or
							// encrypted, which we don't support
							log_error( "%S : compression and/or encryption are NOT supported in ZIP files : [%s]", __FUNCTION__, zip_filename.c_str() );
						}
					}
					// central directory file header
					else if( *( (int*) rptr ) == 0x02014b50 )
					{
						// once we hit the central directory, we're done with file entries
						// and can stop reading the ZIP file
						break;
					}
				}
			}
		}
	}
}

bool w_io_zip::does_toc_contain_filename( std::string filename )
{
	if( table_of_contents.count( filename ) > 0 )
	{
		return true;
	}

	return false;
}

w_zip_toc_entry* w_io_zip::get_toc_entry_for_filename( std::string filename )
{
	if( !does_toc_contain_filename( filename ) )
	{
		return nullptr;
	}

	return table_of_contents[filename].get();
}

std::unique_ptr<std::vector<char>> w_io_zip::get_data_for_filename( std::string filename )
{
	w_zip_toc_entry* toc_entry = get_toc_entry_for_filename( filename );

	if( toc_entry )
	{
		FILE* f;
		fopen_s( &f, toc_entry->zip_filename.c_str(), "rb" );

		if( f )
		{
			fseek( f, toc_entry->offset_from_start_of_file, 0 );
			std::unique_ptr<std::vector<char>> buffer = std::make_unique<std::vector<char>>( toc_entry->size );
			fread_s( buffer->data(), toc_entry->size, 1, toc_entry->size, f );
			fclose( f );

			return std::move( buffer );
		}
	}

	return nullptr;
}
