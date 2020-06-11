#pragma once

// ----------------------------------------------------------------------------

struct w_file_system
{
	std::unique_ptr<w_io_zip> zip_io;

	w_file_system();
	void init();

	bool file_exists_on_disk( const std::string& filename );
	bool file_exists_on_disk_or_in_zip( const std::string& filename );
	std::unique_ptr<w_mem_file> load_file_into_memory( std::string filename );
	void scan_folder_for_ext( std::vector<std::string>& filenames, std::string folder, std::string extension );
};
