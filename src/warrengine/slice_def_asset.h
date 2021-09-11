
namespace war
{

struct Slice_Def_Asset : Asset
{
	std::array<Texture_Asset*, 9> patches = {};

	float_t get_left_slice_sz() const;
	float_t get_right_slice_sz() const;
	float_t get_top_slice_sz() const;
	float_t get_bottom_slice_sz() const;
};

}
