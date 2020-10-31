
#include "app_header.h"

w_artist::w_artist( const std::string& name )
	: name( name )
{
}

// ----------------------------------------------------------------------------

const char* work_log_tracker_game::data_filename = "work_log_tracker.data";
const char* work_log_tracker_game::artist_filename = "artist_list.txt";

work_log_tracker_game::work_log_tracker_game( const char* name )
	: w_game( name )
{
}

void work_log_tracker_game::init()
{
	auto file = engine->fs->load_text_file_into_memory( engine->fs->prepend_data_path( work_log_tracker_game::artist_filename ) );

	for( const auto& artist : *( file->lines.get() ) )
	{
		artists.emplace_back( artist );
	}

	engine->layer_mgr->push<layer_background>();
	engine->layer_mgr->push<layer_edit_list>();

	engine->window->set_mouse_mode( mouse_mode::custom );
}

void work_log_tracker_game::load_data()
{
	if( engine->fs->file_exists_on_disk( work_log_tracker_game::data_filename ) )
	{
		w_file_disk file;
		file.open_for_read( work_log_tracker_game::data_filename );

		bool success = false;
		for( auto& artist : artists )
		{
			 success = file.read_glob( &( artist.approved ), sizeof( bool ) );

			if( !success )
			{
				break;
			}
		}

		if( !success )
		{
			for( auto& artist : artists )
			{
				artist.approved = false;
			}
		}

		file.close();
	}
}

void work_log_tracker_game::save_data()
{
	w_file_disk file;
	file.open_for_write( work_log_tracker_game::data_filename );

	for( auto& artist : artists )
	{
		file.write_glob( &( artist.approved ), sizeof( bool ) );
	}

	file.close();
}
