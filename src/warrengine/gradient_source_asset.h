
namespace war
{

struct Gradient_Source_Asset : Texture_Source_Base
{
	std::vector<float> colors = {};
	e_align_t alignment = e_align::horizontal;

	virtual bool create() override;
};

}
