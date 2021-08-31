
namespace war
{

// ----------------------------------------------------------------------------

struct Perlin_Noise
{
	float idx = 0.f;
	float seed = 0.f;

	[[nodiscard]] float get();
};

// ----------------------------------------------------------------------------

struct Simplex_Noise
{
	float idx = 0.f;
	float seed = 0.f;

	[[nodiscard]] float get();
};

// ----------------------------------------------------------------------------

struct Random_Noise
{
	float idx = 0.f;
	float seed = 0.f;

	[[nodiscard]] float get();
};

}
