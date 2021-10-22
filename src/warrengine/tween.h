
namespace war
{

struct Tween final
{
	tweeny::tween<f32> _tween = {};
	f32 start = 0.f, end = 1.f;

	time_ms time_last;

	Tween();
	Tween( f32 start, f32 end, time_ms duration_ms, e_tween_type type, e_tween_via via );

	[[nodiscard]] f32 operator*();
	[[nodiscard]] f32 value();

	void restart();
	void randomize();
	void toggle_direction();
	void set_backwards();
	void set_type( e_tween_type type );
	void set_via( e_tween_via via );
};

}
