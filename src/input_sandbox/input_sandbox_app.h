#pragma once

// ----------------------------------------------------------------------------

struct input_sandbox_app : w_game
{
	input_sandbox_app( const char* name );

	void init() override;
	void new_game() override;
	void update() override;
};
