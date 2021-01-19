#pragma once

struct w_bit_flag_generator final
{
	unsigned bit = 1;

	w_bit_flag_generator() = default;
	w_bit_flag_generator( unsigned start_bit );
	void reset();
	[[nodiscard]] unsigned next();
};
