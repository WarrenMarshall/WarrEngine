
#include "master_pch.h"
#include "master_header.h"

namespace war
{

Bit_Flag_Generator::Bit_Flag_Generator( uint16 start_bit )
{
	bit = start_bit;
}

// returns current value
uint16 Bit_Flag_Generator::get()
{
	return bit;
}

// bit shifts to the next value, then returns it
uint16 Bit_Flag_Generator::next()
{
	bit = bit << 1;
	return bit;
}
}
