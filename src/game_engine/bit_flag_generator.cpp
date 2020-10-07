#include "master_pch.h"
#include "master_header.h"

w_bit_flag_generator::w_bit_flag_generator( unsigned start_bit )
{
	bit = start_bit;
}

void w_bit_flag_generator::reset()
{
	bit = 1;
}

unsigned w_bit_flag_generator::next()
{
	auto ret = bit;
	bit = bit << 1;
	return ret;
}
