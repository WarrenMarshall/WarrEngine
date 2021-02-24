#pragma once

// ----------------------------------------------------------------------------

#include "file_zip.h"

struct w_file_system
{
	std::unique_ptr<w_file_zip> zip_io;

	w_file_system();
	void init();

	static bool file_exists_on_disk( const std::string_view filename );
	static void create_path_if_not_exist( std::string_view path );

	[[nodiscard]] bool file_exists_on_disk_or_in_zip( const std::string_view filename );
	[[nodiscard]] std::unique_ptr<w_file_mem> load_binary_file( const std::string_view filename );
	[[nodiscard]] std::unique_ptr<w_file_mem_text> load_text_file( std::string_view filename );
	void scan_folder_for_ext( std::vector<std::string>* filenames, const std::string_view folder, const std::string_view extension );
	[[nodiscard]] std::string prepend_data_path( std::string_view base_path );

	// loads a file into memory, contained in a w_mem_file object. the lifetime of
	// this object will be managed by the caller.
	//
	// - first, look at the hard disk to see if the file is there.
	//		- if it is, load it and we're done
	//		- if it is NOT, load it from the ZIP files found in our folder
	//		- if it's not in there either, then it doesn't exist - ERROR!
	//
	// this pattern means that files on disk override files in ZIPs. this allows you
	// to work on loose files during development and then ZIP everything up for
	// release.

	template <typename T>
	std::unique_ptr<T> load_file_into_memory( std::string_view filename )
	{
		// look on hard drive

		if( w_file_system::file_exists_on_disk( filename ) )
		{
			// file found, load it

			std::ifstream file( filename.data(), std::ios::binary | std::ios::ate );
			std::streamsize size = file.tellg();
			file.seekg( 0, std::ios::beg );

			std::unique_ptr<T> mem_file = std::make_unique<T>( static_cast<int>( size ) );
			file.read( &( *mem_file->buffer )[ 0 ], size );

			return std::move( mem_file );
		}
		else
		{
			// look in ZIP files

			w_zip_toc_entry* toc_entry = zip_io->get_toc_entry_for_filename( filename );
			if( toc_entry == nullptr )
			{
				// file not found in a ZIP file either - ERROR!
				log_error( "File not found on disk OR in a zip file : [{}]", filename );
			}

			// file found in table of contents, load it from ZIP

			auto mem_file = std::make_unique<T>( toc_entry->size );
			mem_file->buffer = zip_io->get_data_for_filename( filename );

			return std::move( mem_file );
		}
	}
};
