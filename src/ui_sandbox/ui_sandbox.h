#pragma once

// ----------------------------------------------------------------------------

struct ui_sandbox : w_game
{
	ui_sandbox( std::string_view name );

	void init() override;
	void new_game() override;
	void update() override;
};
