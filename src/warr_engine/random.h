#pragma once

struct w_random
{
	std::random_device rd;
	std::mt19937 gen;
	std::uniform_real_distribution<float> distr;

	w_random();

    [[nodiscard]] float getf();
    [[nodiscard]] bool getb();
    [[nodiscard]] float getf_range( float min, float max );
    [[nodiscard]] int geti_range( int min, int max );
	[[nodiscard]] w_vec2 get_random_unit_circle();
};
