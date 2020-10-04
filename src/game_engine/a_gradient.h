#pragma once

struct a_gradient : a_texture
{
	virtual ~a_gradient() override;

	e_align alignment = align::horizontal;
	std::vector<float> colors;

	virtual void clean_up_internals() override;
	virtual bool create_internals() override;
};

