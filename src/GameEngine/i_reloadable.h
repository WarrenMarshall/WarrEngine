#pragma once

struct i_reloadable
{
	// files loaded from ZIP files are not eligible for hot reloading
	bool was_loaded_from_zip_file = false;

	// the filename this resource was loaded from originally
	std::string original_filename = "";

	// the "last modified" date/time for this resources file on disk
	long long last_modified = 0;

	long long get_last_modified_from_disk();
	bool needs_reloading();

	virtual void clean_up_internals() = 0;
	virtual bool create_internals( bool is_hot_reloading ) = 0;
};
