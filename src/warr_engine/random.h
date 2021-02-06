#pragma once

struct w_random
{
	std::random_device rd;
	std::mt19937 gen;
	std::uniform_real_distribution<float> distr;

	w_random();

	_NODISCARD float getf();
	_NODISCARD bool getb();
	_NODISCARD float getf_range( float min, float max );
	_NODISCARD int geti_range( int min, int max );
	_NODISCARD w_vec2 get_random_unit_circle();
};
