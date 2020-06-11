#pragma once

// ----------------------------------------------------------------------------

struct i_asset : i_reloadable
{
	std::string name = "";
	std::unique_ptr<w_render_buffer> render_buffer = nullptr;

	virtual void clean_up_internals();
	virtual bool create_internals( bool is_hot_reloading );
	virtual void update();
	virtual void update_fts();
	virtual void draw( e_render_pass render_pass );
	virtual void clear_render_buffer();
};

// ----------------------------------------------------------------------------
