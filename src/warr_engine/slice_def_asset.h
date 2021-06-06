
namespace war
{

struct slice_def_asset final : asset
{
	std::array<texture_asset*, 9> patches = {};

	float get_left_slice_sz() const;
	float get_right_slice_sz() const;
	float get_top_slice_sz() const;
	float get_bottom_slice_sz() const;
};

}
