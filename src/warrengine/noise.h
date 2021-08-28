
namespace war
{

// ----------------------------------------------------------------------------

struct Perlin_Noise final
{
	float idx = 0.f;
	float seed = 0.f;

	[[nodiscard]] float get();
};

// ----------------------------------------------------------------------------

struct Simplex_Noise final
{
	float idx = 0.f;
	float seed = 0.f;

	[[nodiscard]] float get();
};

// ----------------------------------------------------------------------------

struct Random_Noise final
{
	float idx = 0.f;
	float seed = 0.f;

	[[nodiscard]] float get();
};

}
