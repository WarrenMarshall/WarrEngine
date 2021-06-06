
#include "master_pch.h"
#include "master_header.h"

namespace war
{

bit_flag_generator::bit_flag_generator( uint16 start_bit )
{
	bit = start_bit;
}

void bit_flag_generator::reset()
{
	bit = 1;
}

unsigned bit_flag_generator::next()
{
	auto ret = bit;
	bit = bit << 1;
	return ret;
}

}
