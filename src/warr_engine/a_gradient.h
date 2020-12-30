#pragma once

#if 0 // #texture
struct a_gradient : a_src_texture
{
	declare_find_func( a_gradient )

	virtual ~a_gradient() override;

	std::vector<float> colors;
	e_align alignment = align::horizontal;

	virtual void clean_up_internals() override;
	virtual bool create_internals() override;
};

#endif
