#pragma once

// ----------------------------------------------------------------------------

struct i_asset
{
	virtual ~i_asset( ) = default;

	std::string tag;
	std::string original_filename;

	virtual void clean_up_internals();
	virtual bool create_internals();
	virtual void finalize_after_loading();

	virtual void update() {};
	virtual void adjust_for_time_dilation() {}
};

// ----------------------------------------------------------------------------
