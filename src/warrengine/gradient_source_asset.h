
namespace war
{

struct Gradient_Source_Asset final : Texture_Source_Base
{
	std::vector<float> colors = {};
	e_align alignment = align::horizontal;

	virtual bool create() override;
};

}
