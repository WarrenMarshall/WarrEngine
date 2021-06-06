
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------
// the ZIP system works as follows:
//
// 1. the ZIP file containing the data files cannot use any compression or
//    encryption. the engine doesn't have any idea what to do with that. files
//    must be stored in the ZIP, that's it.
//
// 2. The ZIP file must be sitting in the same folder as the EXE.
//
// 3. the ZIP file must have "data" at the root of it's file structure. this
//    directly mimics the file structure used during development.
//
// 	i.e. data/warr_engine/filename.asset_def data/my_game/gfx/filename.png
// 	...etc...
// ----------------------------------------------------------------------------

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

							std::string wk_filename = filename;
							std::replace( wk_filename.begin(), wk_filename.end(), '\\', '/' );

							table_of_contents.insert(
								std::make_pair(
									wk_filename,
									w_zip_toc_entry( zip_filename, wk_filename, (int) ( rptr - buffer.data() ), hdr->uncompressed_size )
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
							// Anything else means the entry is compressed
							// and/or encrypted, which we don't support
							log_error( "Compression and/or encryption are NOT supported in ZIP files : [{}]", zip_filename );
						}
					}
					// central directory file header
					else if( *( (int*) rptr ) == 0x02014b50 )
					{
						// once we hit the central directory, we're done with
						// file entries and can stop reading the ZIP file
						break;
					}
				}
			}
		}
	}
}

w_zip_toc_entry* w_file_zip::get_toc_entry_for_filename( std::string_view filename )
{
	auto iter = table_of_contents.find( std::string( filename ) );

	if( iter == table_of_contents.end() )
	{
		return nullptr;
	}

	return &( iter->second );
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