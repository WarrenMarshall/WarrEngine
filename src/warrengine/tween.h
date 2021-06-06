
namespace war
{

struct tween
{
	tweeny::tween<float> _tween = {};
	float start = 0.f, end = 1.f;

	time_ms time_last;

	tween();
	tween( float start, float end, time_ms duration_ms, e_tween_type type, e_tween_via via );

	[[nodiscard]] float operator*();

	void restart();
	void randomize();
	void toggle_direction();
	void set_backwards();
	void set_type( e_tween_type type );
	void set_via( e_tween_via via );
};

}
