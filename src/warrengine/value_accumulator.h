
namespace war
{

// ----------------------------------------------------------------------------
// a value that wants to accumulate over a period of time, and then report
// an averaged value for that period.

struct value_accumulator
{
	float value = 0.f;

	void init();
	void update_value( int steps );
	void update_value();

	value_accumulator& operator++();
	value_accumulator operator++( int );
	value_accumulator& operator--();
	value_accumulator operator--( int );
	value_accumulator operator+( float value ) const;
	value_accumulator operator+=( float v );
	value_accumulator operator+( time_ms value ) const;
	value_accumulator operator+=( time_ms v );

private:

	float accumulator = 0.f;
};

}
