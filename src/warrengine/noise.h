
namespace war
{

// ----------------------------------------------------------------------------

struct Perlin_Noise
{
	float_t idx = 0.f;
	float_t seed = 0.f;

	[[nodiscard]] float_t get();
};

// ----------------------------------------------------------------------------

struct Simplex_Noise
{
	float_t idx = 0.f;
	float_t seed = 0.f;

	[[nodiscard]] float_t get();
};

// ----------------------------------------------------------------------------

struct Random_Noise
{
	float_t idx = 0.f;
	float_t seed = 0.f;

	[[nodiscard]] float_t get();
};

}
