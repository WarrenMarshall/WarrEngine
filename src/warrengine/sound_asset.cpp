
#include "master_pch.h"
#include "master_header.h"

namespace war
{

sound_asset::~sound_asset()
{
	sound.stop();
}

void sound_asset::play()
{
	sound.play();
	sound.setPitch( g_engine->time.dilation );
}

void sound_asset::pause()
{
	sound.pause();
}

void sound_asset::stop()
{
	sound.stop();
}

bool sound_asset::create()
{
	auto file = file_system::load_binary_file( original_filename );

	if( buffer.loadFromMemory( file->buffer.data(), file->buffer.size() ) )
	{
		sound.setBuffer( buffer );
		sound.setLoop( looped );

		return true;
	}

	log_fatal( "Couldn't load the file : [{}]", original_filename );
	return false;
}

void sound_asset::adjust_for_time_dilation()
{
	if( sound.getStatus() != sf::SoundSource::Stopped )
	{
		sound.setPitch( g_engine->time.dilation );
	}
}

}
