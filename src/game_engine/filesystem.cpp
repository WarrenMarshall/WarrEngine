#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

w_file_system::w_file_system()
{
	zip_io = std::make_unique<w_file_zip>();
}

void w_file_system::init()
{
	zip_io->scan_and_build_table_of_contents();
}

bool w_file_system::file_exists_on_disk( const std::string_view filename )
{
	return std::filesystem::exists( filename );
}

// checks to see if a filename is valid either on the disk or in a zip file.

bool w_file_system::file_exists_on_disk_or_in_zip( const std::string_view filename )
{
	if( file_exists_on_disk( filename ) )
	{
		return true;
	}

	w_zip_toc_entry* toc_entry = zip_io->get_toc_entry_for_filename( filename );
	if( toc_entry != nullptr )
	{
		return true;
	}

	return false;
}

// loads a file into memory, contained in a w_mem_file object. the lifetime of
// this object will be managed by the caller.
//
// we first look at the hard disk to see if the file is there.
// - if it is, load it and we're done
// - if it is NOT, load it from the ZIP files found in our folder
// - if it's not in there either, then it doesn't exist - CRASH!
//
// this means that files on disk override files in ZIPs. this allows me to
// work on loose files during development and then ZIP everything up for
// release.

std::unique_ptr<w_file_mem> w_file_system::load_file_into_memory( std::string_view filename )
{
	if( file_exists_on_disk( filename ) )
	{
		std::ifstream file( filename.data(), std::ios::binary | std::ios::ate );
		std::streamsize size = file.tellg();
		file.seekg( 0, std::ios::beg );

		std::unique_ptr<w_file_mem> mem_file = std::make_unique<w_file_mem>( static_cast<int>( size ) );
		file.read( &(*mem_file->buffer)[0], size );
		return std::move( mem_file );
	}
	else
	{
		w_zip_toc_entry* toc_entry = zip_io->get_toc_entry_for_filename( filename );
		if( toc_entry == nullptr )
		{
			log_error( "File not found on disk OR in a zip file : [{}]", filename );
		}
		else
		{
			auto mem_file = std::make_unique<w_file_mem>( toc_entry->size );
			mem_file->buffer = zip_io->get_data_for_filename( filename );
			return std::move( mem_file );
		}
	}

	return nullptr;
}

std::unique_ptr<w_mem_file_text> w_file_system::load_text_file_into_memory( std::string_view filename )
{
	if( file_exists_on_disk( filename ) )
	{
		std::ifstream file( filename.data(), std::ios::binary | std::ios::ate );
		std::streamsize size = file.tellg();
		file.seekg( 0, std::ios::beg );

		auto mem_file = std::make_unique<w_mem_file_text>( static_cast<int>( size ) );
		file.read( mem_file->buffer->data(), size );
		mem_file->preprocess();
		return std::move( mem_file );
	}
	else
	{
		w_zip_toc_entry* toc_entry = zip_io->get_toc_entry_for_filename( filename );
		if( toc_entry == nullptr )
		{
			log_error( "File not found on disk OR in a zip file : [{}]", filename );
		}
		else
		{
			auto mem_file = std::make_unique<w_mem_file_text>( toc_entry->size );
			mem_file->buffer = zip_io->get_data_for_filename( filename );
			mem_file->preprocess();
			return std::move( mem_file );
		}
	}

	return nullptr;
}

void w_file_system::scan_folder_for_ext( std::vector<std::string>& filenames, std::string_view folder, std::string_view extension )
{
	// look on disk

	if( std::filesystem::exists( folder ) )
	{
		for( auto& iter : std::filesystem::directory_iterator( folder ) )
		{
			std::string filename = iter.path().string();
			std::string ext = iter.path().filename().extension().string();

			if( ext == extension )
			{
				filenames.emplace_back( filename );
			}
		}
	}

	// look in ZIP files

	for( auto& [filename, toc_entry] : zip_io->table_of_contents )
	{
		w_tokenizer tok( filename, '/' );
		auto foldername = *tok.get_next_token();

		if( foldername == folder )
		{
			std::string new_filename = w_stringutil::replace_char( filename, '\\', '/' );
			std::filesystem::path path = new_filename;

			if( path.extension() == extension )
			{
				if( std::find( filenames.begin(), filenames.end(), new_filename ) == filenames.end() )
				{
					filenames.emplace_back( new_filename );
				}
			}
		}
	}
}
