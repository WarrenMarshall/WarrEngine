#pragma once

struct w_bit_flag_generator
{
	unsigned bit = 1;

	w_bit_flag_generator() = default;
	w_bit_flag_generator( unsigned start_bit );
	void reset();
	_NODISCARD unsigned next();
};
