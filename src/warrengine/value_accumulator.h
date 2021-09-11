
namespace war
{

// ----------------------------------------------------------------------------
// a value that wants to accumulate over a period of time, and then report
// an averaged value for that period.

struct Value_Accumulator
{
	float_t value = 0.f;

	void init();
	void update_value( int32_t steps );
	void update_value();

	Value_Accumulator& operator++();
	Value_Accumulator operator++( int32_t );
	Value_Accumulator& operator--();
	Value_Accumulator operator--( int32_t );
	Value_Accumulator operator+( float_t value ) const;
	Value_Accumulator operator+=( float_t v );
	Value_Accumulator operator+( time_ms value ) const;
	Value_Accumulator operator+=( time_ms v );

private:

	float_t accumulator = 0.f;
};

}
