
namespace war
{

struct Slice_Def_Asset final : Asset
{
	std::array<Texture_Asset*, 9> patches = {};

	f32 get_left_slice_sz() const;
	f32 get_right_slice_sz() const;
	f32 get_top_slice_sz() const;
	f32 get_bottom_slice_sz() const;
};

}
