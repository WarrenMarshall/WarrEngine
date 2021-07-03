
#include "master_pch.h"
#include "master_header.h"

namespace war
{

bit_flag_generator::bit_flag_generator( uint16 start_bit )
{
	bit = start_bit;
}

// returns current value
uint16 bit_flag_generator::get()
{
	return bit;
}

// bit shifts to the next value, then returns it
uint16 bit_flag_generator::next()
{
	bit = bit << 1;
	return bit;
}
}
