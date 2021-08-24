
using namespace war;

struct scene_noise final : scene
{
	scene_noise();

	noise_simplex simplex = {};
	float simplex_samples[ 64 ];

	noise_perlin perlin = {};
	float perlin_samples[ 64 ];

	noise_random random = {};
	float random_samples[ 64 ];

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;
	void generate_noise_samples();
};
