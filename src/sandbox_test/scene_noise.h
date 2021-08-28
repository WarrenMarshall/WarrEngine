
using namespace war;

struct scene_noise final : scene
{
	scene_noise();

	Simplex_Noise simplex = {};
	float simplex_samples[ 64 ];

	Perlin_Noise perlin = {};
	float perlin_samples[ 64 ];

	Random_Noise random = {};
	float random_samples[ 64 ];

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;
	void generate_noise_samples();
};
