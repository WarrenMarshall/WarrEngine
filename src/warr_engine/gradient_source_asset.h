
namespace war
{

struct gradient_source_asset final : texture_source_base
{
	std::vector<float> colors = {};
	e_align alignment = align::horizontal;

	virtual bool create() override;
};

}
