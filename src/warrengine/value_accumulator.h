
namespace war
{

// ----------------------------------------------------------------------------
// a value that wants to accumulate over a period of time, and then report
// an averaged value for that period.

struct Value_Accumulator final
{
	f32 value = 0.f;

	void init();
	void update_value( i32 steps );
	void update_value();

	Value_Accumulator& operator++();
	Value_Accumulator operator++( i32 );
	Value_Accumulator& operator--();
	Value_Accumulator operator--( i32 );
	Value_Accumulator operator+( f32 value ) const;
	Value_Accumulator operator+=( f32 v );
	Value_Accumulator operator+( time_ms value ) const;
	Value_Accumulator operator+=( time_ms v );

private:

	f32 accumulator = 0.f;
};

}
