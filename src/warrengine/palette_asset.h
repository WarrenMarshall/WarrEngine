
namespace war
{

struct palette_asset final : asset
{
	std::vector<color> colors;

	const color& operator []( size_t idx ) const
	{
		assert( colors.size() );
		assert( idx >= 0 and idx < colors.size() );

		return colors[ idx ];
	}
};

}
