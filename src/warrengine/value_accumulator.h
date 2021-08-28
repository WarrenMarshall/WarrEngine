
namespace war
{

// ----------------------------------------------------------------------------
// a value that wants to accumulate over a period of time, and then report
// an averaged value for that period.

struct Value_Accumulator final
{
	float value = 0.f;

	void init();
	void update_value( int steps );
	void update_value();

	Value_Accumulator& operator++();
	Value_Accumulator operator++( int );
	Value_Accumulator& operator--();
	Value_Accumulator operator--( int );
	Value_Accumulator operator+( float value ) const;
	Value_Accumulator operator+=( float v );
	Value_Accumulator operator+( time_ms value ) const;
	Value_Accumulator operator+=( time_ms v );

private:

	float accumulator = 0.f;
};

}
