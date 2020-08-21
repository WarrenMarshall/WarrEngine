#pragma once

// ----------------------------------------------------------------------------

struct w_artist
{
	std::string name;
	struct 
	{
		unsigned approved : 1 = false;
	};
	
	w_artist( const std::string& name );
};

struct work_log_tracker_game final : w_game
{
	work_log_tracker_game();

	std::vector<w_artist> artists;

	void init() final;
	void new_game() final;
	void update() final;
};
