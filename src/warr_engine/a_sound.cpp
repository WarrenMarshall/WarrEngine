
#include "master_pch.h"
#include "master_header.h"

implement_find_func( a_sound )

a_sound::~a_sound()
{
	a_sound::clean_up_internals();
}

void a_sound::clean_up_internals()
{
	if( sample_handle > 0 )
	{
		BASS_SampleFree( sample_handle );
		sample_handle = 0;
	}
}

void a_sound::play()
{
	if( looped )
	{
		channel_handle = BASS_SampleGetChannel( sample_handle, false );
		BASS_ChannelPlay( channel_handle, false );
	}
	else
	{
		BASS_ChannelPlay( channel_handle, true );
	}
}

void a_sound::stop()
{
	if( channel_handle )
	{
		BASS_ChannelStop( channel_handle );
		channel_handle = 0;
	}
}

bool a_sound::create_internals()
{
	bool file_exists = engine->fs->file_exists_on_disk_or_in_zip( original_filename );

	if( !file_exists )
	{
		log_error( "Couldn't find the file : [{}]", tag );
	}

	auto file = engine->fs->load_file_into_memory( original_filename );

	sample_handle = BASS_SampleLoad(
		true,
		file->buffer->data(),
		0,
		static_cast<int>( file->buffer->size() ),
		looped ? 1 : 32,
		BASS_SAMPLE_OVER_VOL | (looped ? BASS_SAMPLE_LOOP : 0)
	);

	if( !sample_handle && BASS_IsStarted() )
	{
		log_error( "Couldn't load the file : [{}]", original_filename );
	}

	channel_handle = BASS_SampleGetChannel( sample_handle, false );

	return true;
}
