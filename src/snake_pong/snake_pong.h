#pragma once

// ----------------------------------------------------------------------------

struct snake_pong : w_game
{
	snake_pong( const char* name );

	virtual void init() override;
	virtual void reset_layer_stack_to_main_menu() override;
	virtual void new_game() override;
};
