#pragma once

struct a_sound : i_asset
{
	declare_find_func( a_sound )

#ifdef USE_BASS_SOUND_LIBRARY
	int snd = -1;
	int channel = -1;
#else
	cs_loaded_sound_t snd = {};
	cs_playing_sound_t* snd_playing = nullptr;
#endif
	bool looped = false;

	virtual ~a_sound() override;

	void play();
	void stop();
	virtual void clean_up_internals() override;
	virtual bool create_internals() override;
};
