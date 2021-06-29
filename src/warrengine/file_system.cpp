
#include "master_pch.h"
#include "master_header.h"

namespace war
{

void file_system::init()
{
	file_system::zip_file.scan_and_build_table_of_contents();
}

bool file_system::file_exists_on_disk( std::string_view filename )
{
	return std::filesystem::exists( filename );
}

// checks to see if a filename is valid either on the disk or in a zip file.

bool file_system::file_exists_on_disk_or_in_zip( std::string_view filename )
{
	if( file_system::file_exists_on_disk( filename ) )
	{
		return true;
	}

	auto toc_entry = file_system::zip_file.get_toc_entry_for_filename( filename );
	if( toc_entry != nullptr )
	{
		return true;
	}

	return false;
}

std::unique_ptr<file_mem> file_system::load_binary_file( std::string_view filename )
{
	return load_file_into_memory<file_mem>( filename );
}

std::unique_ptr<file_mem_text> file_system::load_text_file( std::string_view filename )
{
	auto file = load_file_into_memory<file_mem_text>( filename );
	file->preprocess();
	return file;
}

std::unique_ptr<file_mem_text> file_system::load_text_file_raw( std::string_view filename )
{
	auto file = load_file_into_memory<file_mem_text>( filename );
	file->preprocess_raw();

	return file;
}

void file_system::scan_folder_for_ext( std::vector<std::string>* filenames, std::string_view folder, std::string_view extension )
{
	// look on disk

	if( std::filesystem::exists( folder ) )
	{
		for( auto& iter : std::filesystem::directory_iterator( folder ) )
		{
			if( !iter.is_directory() )
			{
				std::string filename = iter.path().string();
				std::string ext = iter.path().filename().extension().string();

				if( ext == extension )
				{
					filenames->push_back( filename );
				}
			}
		}
	}

	// look in ZIP files

	for( auto& [filename, toc_entry] : file_system::zip_file.table_of_contents )
	{
		std::string wk_filename = filename;
		string_util::replace_char( wk_filename, '\\', '/' );

		if( wk_filename.substr( 0, folder.size() ) == folder )
		{
			std::filesystem::path path = wk_filename;
			if( path.extension() == extension )
			{
				if( std::find( filenames->begin(), filenames->end(), wk_filename ) == filenames->end() )
				{
					filenames->push_back( wk_filename );
				}
			}
		}
	}
}

// a safety mechanism that will create a folder path on disk
// if it doesn't exist. this helps ensure that things like save
// files will always find a place to write themselves even if
// all the other data files are stored in ZIPs, for example.

void file_system::create_path_if_not_exist( std::string_view path )
{
	if( !std::filesystem::exists( path ) )
	{
		std::filesystem::path wk_path = path;

		if( wk_path.has_extension() )
		{
			wk_path = wk_path.remove_filename();
		}

		std::filesystem::create_directories( wk_path );
	}
}

}
