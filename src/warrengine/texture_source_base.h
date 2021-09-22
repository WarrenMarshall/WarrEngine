
namespace war
{
	struct Texture_Source_Base : Asset
	{
		uint32_t gl_id = 0;
		float_t w = 0.f;
		float_t h = 0.f;

		virtual ~Texture_Source_Base() override;
	};
}
