
namespace war
{

struct asset
{
	virtual ~asset() = default;

	std::string tag;
	std::string original_filename;

	virtual bool create();
	virtual void adjust_for_time_dilation();
};

}

