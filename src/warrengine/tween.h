
namespace war
{

struct Tween
{
	tweeny::tween<float> _tween = {};
	float start = 0.f, end = 1.f;

	time_ms time_last;

	Tween();
	Tween( float start, float end, time_ms duration_ms, e_tween_type_t type, e_tween_via_t via );

	[[nodiscard]] float operator*();

	void restart();
	void randomize();
	void toggle_direction();
	void set_backwards();
	void set_type( e_tween_type_t type );
	void set_via( e_tween_via_t via );
};

}
