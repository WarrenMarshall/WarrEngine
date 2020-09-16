#pragma once

// ----------------------------------------------------------------------------

struct w_artist
{
	std::string name;
	bool approved = false;

	w_artist( const std::string& name );
};

// ----------------------------------------------------------------------------

struct work_log_tracker_game : w_game
{
	work_log_tracker_game( std::string_view name );

	// #todo - can this be static const char* ?
	inline static std::string_view data_filename = "work_log_tracker.data";
	inline static std::string_view artist_filename = "artist_list.txt";
	std::vector<w_artist> artists;

	virtual void init() override;
	virtual void new_game() override;
	virtual void update() override;

	void load_data();
	void save_data();
};
