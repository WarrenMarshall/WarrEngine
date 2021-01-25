
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
	if( w_file_system::file_exists_on_disk( filename ) )
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

std::unique_ptr<w_file_mem> w_file_system::load_binary_file( std::string_view filename )
{
	return load_file_into_memory<w_file_mem>( filename );
}

std::unique_ptr<w_file_mem_text> w_file_system::load_text_file( std::string_view filename )
{
	auto file = load_file_into_memory<w_file_mem_text>( filename );
	file->preprocess();
	return file;
}

void w_file_system::scan_folder_for_ext( std::vector<std::string>* filenames, std::string_view folder, std::string_view extension )
{
	// look on disk

	for( auto& iter : std::filesystem::directory_iterator( folder ) )
	{
		if( !iter.is_directory() )
		{
			std::string filename = iter.path().string();
			std::string ext = iter.path().filename().extension().string();

			if( ext == extension )
			{
				filenames->emplace_back( filename );
			}
		}
	}

	// look in ZIP files

	for( auto& [filename, toc_entry] : zip_io->table_of_contents )
	{
		std::string new_filename = w_string_util::replace_char( filename, '\\', '/' );

		if( new_filename.substr( 0, folder.size() ) == folder )
		{
			std::filesystem::path path = new_filename;
			if( path.extension() == extension )
			{
				if( std::find( filenames->begin(), filenames->end(), new_filename ) == filenames->end() )
				{
					filenames->emplace_back( new_filename );
				}
			}
		}
	}
}

// a safety mechanism that will create a folder path on disk
// if it doesn't exist. this helps ensure that things like save
// files will always find a place to write themselves even if
// all the other data files are stored in ZIPs, for example.

void w_file_system::create_path_if_not_exist( std::string_view path )
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

std::string w_file_system::prepend_data_path( std::string_view base_path )
{
	return fmt::format( "data/{}/{}", base_game->get_game_name(), base_path );
}
