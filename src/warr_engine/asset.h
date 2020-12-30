#pragma once

// ----------------------------------------------------------------------------

struct i_asset
{
	virtual ~i_asset( ) = default;

	std::string tag;
	// #todo - I feel like this should be able to go away - it was really only for hot reloading which is gone now
	std::string original_filename;

	virtual void clean_up_internals();
	virtual bool create_internals();

	virtual void update() {};
};

// ----------------------------------------------------------------------------
