
namespace war
{

struct Palette_Asset final : Asset
{
	std::vector<Color> colors;

	const Color& operator []( size_t idx ) const
	{
		assert( colors.size() );
		assert( idx >= 0 and idx < colors.size() );

		return colors[ idx ];
	}
};

}
