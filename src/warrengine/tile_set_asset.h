
namespace war
{

struct tile_set_asset final : asset
{
	int tile_count = 0;
	int tile_width = 0;
	int tile_height = 0;

	std::vector<texture_asset> tiles;

	virtual bool create() override;
};

}