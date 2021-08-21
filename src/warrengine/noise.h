
namespace war
{

// ----------------------------------------------------------------------------

struct noise_perlin
{
	float idx = 0.f;
	float seed = 0.f;

	[[nodiscard]] float get();
};

// ----------------------------------------------------------------------------

struct noise_simplex
{
	float idx = 0.f;
	float seed = 0.f;

	[[nodiscard]] float get();
};

// ----------------------------------------------------------------------------

struct noise_random
{
	float idx = 0.f;
	float seed = 0.f;

	[[nodiscard]] float get();
};

}
