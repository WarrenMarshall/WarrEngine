
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

void w_accum_value::init()
{
	value = _accumulator = 0.0f;
}

void w_accum_value::update_value( int steps )
{
	value = _accumulator / static_cast<float>( steps );
	_accumulator = 0.0f;
}

void w_accum_value::update_value()
{
	value = _accumulator;
	_accumulator = 0.0f;
}

w_accum_value& w_accum_value::operator++()
{
	_accumulator += 1.0f;
	return *this;
}

w_accum_value w_accum_value::operator++( int )
{
	w_accum_value val( *this );
	++( *this );
	return val;
}

w_accum_value& w_accum_value::operator--()
{
	_accumulator -= 1.0f;
	return *this;
}

w_accum_value w_accum_value::operator--( int )
{
	w_accum_value val( *this );
	--(*this);
	return val;
}

w_accum_value w_accum_value::operator+( float v ) const
{
	w_accum_value av( *this );
	av._accumulator += v;
	return av;
}

w_accum_value w_accum_value::operator+=( float v )
{
	*this = *this + v;
	return *this;
}

w_accum_value w_accum_value::operator+( time_ms v ) const
{
	w_accum_value av( *this );
	av._accumulator += v;
	return av;
}

w_accum_value w_accum_value::operator+=( time_ms v )
{
	*this = *this + v;
	return *this;
}
