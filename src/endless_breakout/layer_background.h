#pragma once

struct layer_background : w_layer
{
	a_gradient* background_gradient = nullptr;
	a_sound* click_snd = nullptr;
	a_sound* snd2 = nullptr;
	a_music* music = nullptr;

	layer_background();

	virtual void push() override;
	virtual void draw() override;
	virtual bool handle_input_event( const w_input_event* evt ) override;
};
