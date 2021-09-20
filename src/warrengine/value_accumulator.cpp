
#include "master_pch.h"
#include "master_header.h"

namespace war
{

void Value_Accumulator::init()
{
	value = accumulator = 0.f;
}

void Value_Accumulator::update_value( int32_t steps )
{
	value = accumulator / (float_t)( steps );
	accumulator = 0.f;
}

void Value_Accumulator::update_value()
{
	value = accumulator;
	accumulator = 0.f;
}

Value_Accumulator& Value_Accumulator::operator++()
{
	accumulator += 1.f;
	return *this;
}

Value_Accumulator Value_Accumulator::operator++( int32_t )
{
	Value_Accumulator val( *this );
	++( *this );
	return val;
}

Value_Accumulator& Value_Accumulator::operator--()
{
	accumulator -= 1.f;
	return *this;
}

Value_Accumulator Value_Accumulator::operator--( int32_t )
{
	Value_Accumulator val( *this );
	--( *this );
	return val;
}

Value_Accumulator Value_Accumulator::operator+( float_t v ) const
{
	Value_Accumulator av( *this );
	av.accumulator += v;
	return av;
}

Value_Accumulator Value_Accumulator::operator+=( float_t v )
{
	*this = *this + v;
	return *this;
}

Value_Accumulator Value_Accumulator::operator+( time_ms v ) const
{
	Value_Accumulator av( *this );
	av.accumulator += v;
	return av;
}

Value_Accumulator Value_Accumulator::operator+=( time_ms v )
{
	*this = *this + v;
	return *this;
}

}
