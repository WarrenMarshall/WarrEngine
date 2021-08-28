#pragma once

struct Layer_Gameplay final : w_layer
{
	Layer_Gameplay();

	virtual void push() override;
	virtual void draw() override;

	virtual bool handle_input_event( const w_input_event* evt ) override;
};
