#pragma once

struct a_sound : i_asset
{
	declare_find_func( a_sound )

	//HSAMPLE sample_handle = 0;
	//HCHANNEL channel_handle = 0;
	bool looped = false;

	sf::Sound sound;
	sf::SoundBuffer buffer;

	virtual ~a_sound() override;

	void play();
	void pause();
	void stop();
	virtual void clean_up_internals() override;
	virtual bool create_internals() override;
	virtual void adjust_for_time_dilation() override;
};
