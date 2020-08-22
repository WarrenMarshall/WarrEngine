#pragma once

// ----------------------------------------------------------------------------

struct w_artist
{
	std::string name;
	bool approved = false;
	
	w_artist( const std::string& name );
};

struct work_log_tracker_game final : w_game
{
	work_log_tracker_game();

	std::string data_filename = "work_log_tracker.data";
	std::vector<w_artist> artists;

	void init() final;
	void new_game() final;
	void update() final;

	void load_data();
	void save_data();
};
