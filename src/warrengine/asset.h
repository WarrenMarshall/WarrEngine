
namespace war
{

// an asset is anything that lives in the asset cache. typically this is stuff
// loaded from disk that the game/app needs like textures or sounds.

struct Asset
{
	virtual ~Asset() = default;

	std::string tag;
	std::string original_filename;

	virtual bool create();
	virtual void adjust_for_time_dilation();
};

}

