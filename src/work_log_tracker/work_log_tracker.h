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
	work_log_tracker_game( const char* name );

	static const char* data_filename;
	static const char* artist_filename;

	std::vector<w_artist> artists;

	virtual void init() override;

	void load_data();
	void save_data();
};
