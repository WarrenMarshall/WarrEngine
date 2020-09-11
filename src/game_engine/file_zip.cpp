#include "master_pch.h"
#include "master_header.h"

w_zip_toc_entry::w_zip_toc_entry( std::string_view zip_filename, std::string_view filename, int offset, int size )
	: zip_filename( zip_filename ), filename( filename ), offset_from_start_of_file( offset ), size( size )
{
}

// ----------------------------------------------------------------------------

void w_file_zip::scan_and_build_table_of_contents()
{
	table_of_contents.clear();

	std::string zip_filename, ext;

	// iterate all the files in the active folder
	for( auto& iter : std::filesystem::directory_iterator( "." ) )
	{
		zip_filename = iter.path().string();
		ext = iter.path().filename().extension().string();

		// for each zip file we find...
		if( ext == ".zip" )
		{
			// read the zip file into memory for quick parsing via pointer arithmetic

			std::ifstream file( zip_filename.data(), std::ios::binary | std::ios::ate );
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
					auto hdr = (zip_local_file_header*) rptr;

					// we don't support compression or encryption in any form
					if( hdr->compression_method != 0 )
					{
						log_error( "Compression and/or encryption are NOT supported in ZIP files : [{}]", zip_filename );
					}

					if( hdr->local_file_header_signature == 0x04034b50 )
					{
						rptr += sizeof( zip_local_file_header );

						if( hdr->version_needed_to_extract == 10 )
						{
							memset( &filename[0], 0, FILENAME_MAX );
							strncpy_s( &filename[0], FILENAME_MAX, rptr, (size_t) hdr->file_name_length );

							rptr += hdr->file_name_length;

							std::string new_filename = filename;
							new_filename = w_stringutil::replace_char( new_filename, '\\', '/' );

							table_of_contents.insert(
								std::make_pair(
									new_filename,
									std::make_unique<w_zip_toc_entry>( zip_filename, new_filename, (int) ( rptr - buffer.data() ), hdr->uncompressed_size )
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
							log_error( "Compression and/or encryption are NOT supported in ZIP files : [{}]", zip_filename );
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

bool w_file_zip::does_toc_contain_filename( std::string_view filename )
{
	if( table_of_contents.count( std::string( filename ) ) > 0 )
	{
		return true;
	}

	return false;
}

w_zip_toc_entry* w_file_zip::get_toc_entry_for_filename( std::string_view filename )
{
	if( !does_toc_contain_filename( filename ) )
	{
		return nullptr;
	}

	return table_of_contents[ std::string( filename ) ].get();
}

std::unique_ptr<std::vector<char>> w_file_zip::get_data_for_filename( std::string_view filename )
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
