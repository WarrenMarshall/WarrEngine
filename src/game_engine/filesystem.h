#pragma once

// ----------------------------------------------------------------------------

struct w_file_system
{
	std::unique_ptr<w_file_zip> zip_io;

	w_file_system();
	void init();

	[[nodiscard]] bool file_exists_on_disk( const std::string_view filename );
	[[nodiscard]] bool file_exists_on_disk_or_in_zip( const std::string_view filename );
	[[nodiscard]] std::unique_ptr<w_file_mem> load_file_into_memory( const std::string_view filename );
	[[nodiscard]] std::unique_ptr<w_mem_file_text> load_text_file_into_memory( std::string_view filename );
	void scan_folder_for_ext( std::vector<std::string>* filenames, const std::string_view folder, const std::string_view extension );
	void create_path_if_not_exist( std::string_view path );
	[[nodiscard]] std::string prepend_data_path( std::string_view base_path );
};
