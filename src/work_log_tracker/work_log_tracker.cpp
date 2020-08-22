
#include "app_header.h"

w_artist::w_artist( const std::string& name )
	: name( name )
{
}

// ----------------------------------------------------------------------------

work_log_tracker_game::work_log_tracker_game()
{
	artists.push_back( w_artist( "Sommer Bostick" ) );
	artists.push_back( w_artist( "Frederick Brown" ) );
	artists.push_back( w_artist( "Jorge Fuentes" ) );
	artists.push_back( w_artist( "Michael Gamez" ) );
	artists.push_back( w_artist( "Dawoda Kah" ) );
	artists.push_back( w_artist( "Adam Linstad" ) );
	artists.push_back( w_artist( "Spencer Matsuura" ) );
	artists.push_back( w_artist( "Ruben Navarez" ) );
	artists.push_back( w_artist( "Fumi Oshodi" ) );
	artists.push_back( w_artist( "Ryan Tabatabai" ) );
	artists.push_back( w_artist( "Robert Rangel" ) );
	artists.push_back( w_artist( "Marco Torres" ) );
	artists.push_back( w_artist( "David Tremback" ) );
	artists.push_back( w_artist( "Alex Whitt" ) );
}

void work_log_tracker_game::init()
{
	UI->set_mouse_visible( true );
}

void work_log_tracker_game::new_game()
{
	w_game::new_game();

	engine->layer_mgr->push( std::make_unique<layer_background>() );
	engine->layer_mgr->push( std::make_unique<layer_edit_list>() );
}

void work_log_tracker_game::update()
{
	w_game::update();
}

void work_log_tracker_game::load_data()
{
	if( engine->fs->file_exists_on_disk( data_filename ) )
	{
		w_file_disk file;
		file.open_for_read( data_filename );

		for( auto& artist : artists )
		{
			file.read_glob( &( artist.approved ), sizeof( bool ) );
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
