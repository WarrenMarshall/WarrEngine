#include "master_pch.h"
#include "master_header.h"

a_sound::~a_sound()
{
	a_sound::clean_up_internals();
}

void a_sound::clean_up_internals()
{
#ifdef USE_BASS_SOUND_LIBRARY
	if( snd > -1 )
	{
		BASS_SampleFree( snd );
		snd = -1;
	}
#else
	if( engine->c2_sound_context && snd.sample_rate )
	{
		cs_free_sound( &snd );
		ZeroMemory( &snd, sizeof( snd ) );
	}
#endif
}

void a_sound::play()
{
#ifdef USE_BASS_SOUND_LIBRARY

	if( looped )
	{
		channel = BASS_SampleGetChannel( snd, false );
	}

	BASS_ChannelPlay( channel, false );

	if( looped )
	{
		BASS_ChannelFlags( channel, BASS_SAMPLE_LOOP, BASS_SAMPLE_LOOP );
	}

#else
	if( engine->c2_sound_context && snd.sample_rate )
	{
		cs_play_sound_def_t def = cs_make_def( &snd );

		if( looped )
		{
			def.looped = 1;
		}

		snd_playing = cs_play_sound( engine->c2_sound_context, def );
	}
#endif
}

void a_sound::stop()
{
#ifdef USE_BASS_SOUND_LIBRARY
	BASS_ChannelStop( channel );
#else
	cs_stop_sound( snd_playing );
#endif
}

bool a_sound::create_internals()
{
	bool file_exists = engine->fs->file_exists_on_disk_or_in_zip( original_filename );

	if( !file_exists )
	{
		log_error( "Couldn't find the file : [{}]", name );
	}

	auto file = engine->fs->load_file_into_memory( original_filename );

#ifdef USE_BASS_SOUND_LIBRARY
	snd = BASS_SampleLoad( true, file->buffer->data(), 0, static_cast<int>( file->buffer->size() ), 1, 0 );

	if( !snd && BASS_IsStarted() )
	{
		log_error( "Couldn't load the file : [{}]", name );
	}

	if( !looped )
	{
		channel = BASS_SampleGetChannel( snd, false );
	}
#else
	cs_read_mem_wav( file->buffer->data(), static_cast<int>( file->buffer->size() ), &snd );

	if( !engine->c2_sound_context && !snd.sample_rate )
	{
		log_error( "Couldn't load the file : [{}]", name );
	}
#endif

	return true;
}

