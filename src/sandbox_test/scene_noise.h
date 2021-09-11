
using namespace war;

struct Scene_Noise : Scene
{
	Scene_Noise();

	Simplex_Noise simplex = {};
	float_t simplex_samples[ 64 ];

	Perlin_Noise perlin = {};
	float_t perlin_samples[ 64 ];

	Random_Noise random = {};
	float_t random_samples[ 64 ];

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;
	void generate_noise_samples();
};
