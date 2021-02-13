
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
	bool file_exists = engine->fs->file_exists_on_disk_or_in_zip( original_filename );

	if( !file_exists )
	{
		log_error( "Couldn't find the file : [{}]", tag );
	}

	auto file = engine->fs->load_binary_file( original_filename );

	if( buffer.loadFromMemory( file->buffer->data(), file->buffer->size() ) )
	{
		sound.setBuffer( buffer );
		sound.setLoop( looped );
	}
	else
	{
		log_error( "Couldn't load the file : [{}]", original_filename );
	}

	return true;
}
