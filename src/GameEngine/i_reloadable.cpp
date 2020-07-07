
#include "master_pch.h"
#include "master_header.h"

std::filesystem::file_time_type i_reloadable::retrieve_last_write_time_from_disk()
{
	std::filesystem::file_time_type blank;

	if( !g_allow_hot_reload )
		return blank;

	if( original_filename.length() == 0 )
		return blank;

	if( was_loaded_from_zip_file )
		return blank;

	std::filesystem::path path = original_filename;
	return std::filesystem::last_write_time( path );
}

bool i_reloadable::needs_reloading()
{
	if( !g_allow_hot_reload )
		return false;

	if( was_loaded_from_zip_file )
		return false;

	std::scoped_lock lock( mutex_last_write_time );
	if( last_write_time_on_disk == last_write_time )
	{
		return false;
	}

	return true;
}
