
namespace war
{

struct noise
{
	noise();

	float idx = 0.f;
	float seed = 0.f;

	void init();
	[[nodiscard]] virtual float get() = 0;
};

// ----------------------------------------------------------------------------

struct noise_perlin : noise
{
	[[nodiscard]] virtual float get() override;
};

// ----------------------------------------------------------------------------

struct noise_simplex : noise
{
	[[nodiscard]] virtual float get() override;
};

// ----------------------------------------------------------------------------

struct noise_random : noise
{
	[[nodiscard]] virtual float get() override;
};

}
