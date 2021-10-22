
namespace war
{

struct Gradient_Source_Asset final : Texture_Source_Base
{
	std::vector<f32> colors = {};
	e_align alignment = e_align::horizontal;

	virtual bool create() override;
};

}
