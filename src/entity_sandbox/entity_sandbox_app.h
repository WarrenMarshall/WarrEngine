#pragma once

// ----------------------------------------------------------------------------

struct entity_sandbox_app : w_game
{
	entity_sandbox_app( std::string_view name );

	virtual void init();
	virtual void new_game();
	virtual void update();
};
