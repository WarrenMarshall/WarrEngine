
#include "app_header.h"

w_artist::w_artist( const std::string& name )
	: name( name )
{
}

// ----------------------------------------------------------------------------

work_log_tracker_game::work_log_tracker_game( std::string_view name )
	: w_game( name )
{
}

void work_log_tracker_game::init()
{
}

void work_log_tracker_game::new_game()
{
	w_game::new_game();

	auto file = engine->fs->load_text_file_into_memory( engine->fs->prepend_data_path( "artist_list.txt" ) );

	for( const auto& artist : *( file.get()->lines.get() ) )
	{
		artists.push_back( w_artist( artist ) );
	}

	engine->layer_mgr->push( std::make_unique<layer_background>() );
	engine->layer_mgr->push( std::make_unique<layer_edit_list>() );
}

void work_log_tracker_game::update()
{
	w_game::update();
}

void work_log_tracker_game::load_data() noexcept
{
	if( engine->fs->file_exists_on_disk( data_filename ) )
	{
		w_file_disk file;
		file.open_for_read( data_filename );

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
	file.open_for_write( data_filename );

	for( auto& artist : artists )
	{
		file.write_glob( &( artist.approved ), sizeof( bool ) );
	}

	file.close();
}
