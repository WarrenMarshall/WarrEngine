
#include "master_pch.h"
#include "master_header.h"

namespace war
{

Bit_Flag_Generator::Bit_Flag_Generator( uint16_t start_bit )
{
	bit = start_bit;
}

// returns current value
uint16_t Bit_Flag_Generator::get()
{
	return bit;
}

// bit shifts to the next value, then returns it
uint16_t Bit_Flag_Generator::next()
{
	bit = bit << 1;
	return bit;
}
}
