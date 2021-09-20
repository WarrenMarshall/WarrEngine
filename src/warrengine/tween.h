
namespace war
{

struct Tween final
{
	tweeny::tween<float_t> _tween = {};
	float_t start = 0.f, end = 1.f;

	time_ms time_last;

	Tween();
	Tween( float_t start, float_t end, time_ms duration_ms, e_tween_type_t type, e_tween_via_t via );

	[[nodiscard]] float_t operator*();
	[[nodiscard]] float_t value();

	void restart();
	void randomize();
	void toggle_direction();
	void set_backwards();
	void set_type( e_tween_type_t type );
	void set_via( e_tween_via_t via );
};

}
