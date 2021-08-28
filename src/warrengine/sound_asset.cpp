
#include "master_pch.h"
#include "master_header.h"

namespace war
{

void Sound_Asset::play()
{
	sound.play();
	sound.setPitch( g_engine->time.dilation );
}

void Sound_Asset::pause()
{
	sound.pause();
}

void Sound_Asset::stop()
{
	sound.stop();
}

bool Sound_Asset::create()
{
	auto file_data = File_System::load_binary_file( original_filename );

	if( buffer.loadFromMemory( file_data->buffer.data(), file_data->buffer.size() ) )
	{
		sound.setBuffer( buffer );
		sound.setLoop( looped );

		return true;
	}

	log_fatal( "Couldn't load the file : [{}]", original_filename );
	return false;
}

void Sound_Asset::adjust_for_time_dilation()
{
	if( sound.getStatus() != sf::SoundSource::Stopped )
	{
		sound.setPitch( g_engine->time.dilation );
	}
}

}
