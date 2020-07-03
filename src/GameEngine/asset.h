#pragma once

// ----------------------------------------------------------------------------

struct i_asset : i_reloadable
{
	std::string name = "";
	std::unique_ptr<w_render_buffer> render_buffer = nullptr;
	std::unique_ptr<w_render_buffer> render_buffer_static = nullptr;

	void clean_up_internals() override;
	bool create_internals( bool is_hot_reloading ) override;

	virtual void update();
	virtual void draw( e_render_pass render_pass );
	virtual void clear_render_buffer();
};

// ----------------------------------------------------------------------------
