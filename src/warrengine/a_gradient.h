#pragma once

struct a_gradient : a_src_texture
{
	declare_find_func( a_gradient )

	std::vector<float> colors = {};
	e_align alignment = align::horizontal;

	virtual bool create_internals() override;
	virtual void finalize_after_loading() override;
};
