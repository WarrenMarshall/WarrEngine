
#include "master_pch.h"
#include "master_header.h"

namespace war
{

bit_flag_generator::bit_flag_generator( uint16 start_bit )
{
	bit = start_bit;
}

uint16 bit_flag_generator::operator++()
{
	bit = bit << 1;
	return bit;
}

uint16 bit_flag_generator::operator++( int )
{
	uint16 val = bit;
	++( *this );
	return val;
}
}
