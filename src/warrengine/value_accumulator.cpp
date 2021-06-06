
#include "master_pch.h"
#include "master_header.h"

namespace war
{

void value_accumulator::init()
{
	value = accumulator = 0.f;
}

void value_accumulator::update_value( int steps )
{
	value = accumulator / (float)( steps );
	accumulator = 0.f;
}

void value_accumulator::update_value()
{
	value = accumulator;
	accumulator = 0.f;
}

value_accumulator& value_accumulator::operator++()
{
	accumulator += 1.f;
	return *this;
}

value_accumulator value_accumulator::operator++( int )
{
	value_accumulator val( *this );
	++( *this );
	return val;
}

value_accumulator& value_accumulator::operator--()
{
	accumulator -= 1.f;
	return *this;
}

value_accumulator value_accumulator::operator--( int )
{
	value_accumulator val( *this );
	--( *this );
	return val;
}

value_accumulator value_accumulator::operator+( float v ) const
{
	value_accumulator av( *this );
	av.accumulator += v;
	return av;
}

value_accumulator value_accumulator::operator+=( float v )
{
	*this = *this + v;
	return *this;
}

value_accumulator value_accumulator::operator+( time_ms v ) const
{
	value_accumulator av( *this );
	av.accumulator += v;
	return av;
}

value_accumulator value_accumulator::operator+=( time_ms v )
{
	*this = *this + v;
	return *this;
}

}
