#pragma once

// ----------------------------------------------------------------------------

struct snake_pong : w_game
{
	snake_pong( const char* name );

	void init() override;
	void new_game() override;
	void update() override;
};
