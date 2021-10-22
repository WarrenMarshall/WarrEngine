
using namespace war;

struct Scene_Noise final : Scene
{
	Scene_Noise();

	Simplex_Noise simplex = {};
	f32 simplex_samples[ 64 ];

	Perlin_Noise perlin = {};
	f32 perlin_samples[ 64 ];

	Random_Noise random = {};
	f32 random_samples[ 64 ];

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;
	void generate_noise_samples();
};
