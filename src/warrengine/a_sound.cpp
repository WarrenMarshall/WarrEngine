
#include "master_pch.h"
#include "master_header.h"

implement_find_func( a_sound )

a_sound::~a_sound()
{
	a_sound::clean_up_internals();

}

void a_sound::clean_up_internals()
{
	sound.stop();
}

void a_sound::play()
{
	sound.play();
	sound.setPitch( engine->time->dilation );
}

void a_sound::pause()
{
	sound.pause();
}

void a_sound::stop()
{
	sound.stop();
}

bool a_sound::create_internals()
{
	auto file = engine->fs->load_binary_file( original_filename );

	if( buffer.loadFromMemory( file->buffer->data(), file->buffer->size() ) )
	{
		sound.setBuffer( buffer );
		sound.setLoop( looped );

		return true;
	}

	log_error( "Couldn't load the file : [{}]", original_filename );
	return false;
}

void a_sound::adjust_for_time_dilation()
{
	if( sound.getStatus() != sf::SoundSource::Stopped )
	{
		sound.setPitch( engine->time->dilation );
	}
}