#pragma once

// ----------------------------------------------------------------------------

struct gametype_sandbox_app : w_game
{
	std::unique_ptr<platformer_physics> plat_physics = nullptr;

	gametype_sandbox_app( std::string_view name );

	virtual void init() override;
	virtual void new_game() override;
	virtual void update() override;
};
