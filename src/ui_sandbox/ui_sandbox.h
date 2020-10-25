#pragma once

// ----------------------------------------------------------------------------

struct ui_sandbox : w_game
{
	ui_sandbox( const char* name );

	void init() override;
	void new_game() override;
	void update() override;
};
