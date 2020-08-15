#pragma once

struct GE_API i_reloadable
{
	// files loaded from ZIP files are not eligible for hot reloading
	bool was_loaded_from_zip_file = false;

	// the filename this resource was loaded from originally
	std::string original_filename = "";

	// the "last modified" date/time for this resources file on disk
	std::mutex mutex_last_write_time;
	std::filesystem::file_time_type last_write_time_on_disk;
	std::filesystem::file_time_type last_write_time;

	std::filesystem::file_time_type retrieve_last_write_time_from_disk();
	bool needs_reloading();

	virtual void clean_up_internals() = 0;
	virtual bool create_internals( bool is_hot_reloading ) = 0;
};
