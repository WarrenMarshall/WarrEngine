
namespace war
{

// ----------------------------------------------------------------------------

struct noise_perlin final
{
	float idx = 0.f;
	float seed = 0.f;

	[[nodiscard]] float get();
};

// ----------------------------------------------------------------------------

struct noise_simplex final
{
	float idx = 0.f;
	float seed = 0.f;

	[[nodiscard]] float get();
};

// ----------------------------------------------------------------------------

struct noise_random final
{
	float idx = 0.f;
	float seed = 0.f;

	[[nodiscard]] float get();
};

}
