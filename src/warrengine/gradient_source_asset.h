
namespace war
{

struct Gradient_Source_Asset final : Texture_Source_Base
{
	std::vector<float_t> colors = {};
	e_align_t alignment = e_align::horizontal;

	virtual bool create() override;
};

}
