
namespace war
{

struct Range final
{
	f32 start = 0.f;
	f32 end = 0.f;

	Range() = default;
	Range( f32 start, f32 end );
	Range( f32 value );
	Range( std::string_view str );

	f32 clamp_value( f32 value );
	[[nodiscard]] f32 get_random_value();
	[[nodiscard]] f32 get_value_at_pct( f32 pct );
	[[nodiscard]] f32 get_pct_from_value( f32 value );
	[[nodiscard]] bool is_within( f32 val );
};

}
