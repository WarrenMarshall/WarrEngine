#pragma once

// ----------------------------------------------------------------------------

struct w_artist
{
	std::string name;
	bool approved = false;

	w_artist( const std::string& name );
};

// ----------------------------------------------------------------------------

struct work_log_tracker_game final : w_game
{
	work_log_tracker_game( std::string_view name );

	// #todo - can this be static const char* ?
	std::string data_filename = "work_log_tracker.data";
	// "artist_list.txt" as well
	std::vector<w_artist> artists;

	virtual void init() final;
	virtual void new_game() final;
	virtual void update() final;

	void load_data() noexcept;
	void save_data();
};
