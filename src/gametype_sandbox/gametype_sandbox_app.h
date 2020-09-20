#pragma once

// ----------------------------------------------------------------------------

struct gametype_sandbox_app : w_game
{
	gametype_sandbox_app( std::string_view name );

	void init() override;
	void new_game() override;
	void update() override;
};
