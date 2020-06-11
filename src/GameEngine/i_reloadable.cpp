
#include "master_pch.h"
#include "master_header.h"

long long i_reloadable::get_last_modified_from_disk()
{
	if( g_allow_hot_reload && !was_loaded_from_zip_file )
	{
		if( original_filename.length() > 0 )
		{
			std::filesystem::path path = original_filename;
			std::filesystem::file_time_type date_time_last_modified = std::filesystem::last_write_time( path );
			return date_time_last_modified.time_since_epoch().count();
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

bool i_reloadable::needs_reloading()
{
	if( !was_loaded_from_zip_file && get_last_modified_from_disk() != last_modified )
		return true;
	
	return false;
}
