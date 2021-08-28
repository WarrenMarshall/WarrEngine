
namespace war
{

struct Slide_Def_Asset final : Asset
{
	std::array<Texture_Asset*, 9> patches = {};

	float get_left_slice_sz() const;
	float get_right_slice_sz() const;
	float get_top_slice_sz() const;
	float get_bottom_slice_sz() const;
};

}
