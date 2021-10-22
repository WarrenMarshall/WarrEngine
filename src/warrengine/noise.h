
namespace war
{

// ----------------------------------------------------------------------------

struct Perlin_Noise final
{
	f32 idx = 0.f;
	f32 seed = 0.f;

	[[nodiscard]] f32 get();
};

// ----------------------------------------------------------------------------

struct Simplex_Noise final
{
	f32 idx = 0.f;
	f32 seed = 0.f;

	[[nodiscard]] f32 get();
};

// ----------------------------------------------------------------------------

struct Random_Noise final
{
	f32 idx = 0.f;
	f32 seed = 0.f;

	[[nodiscard]] f32 get();
};

}
