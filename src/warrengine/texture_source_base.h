
namespace war
{
	// #Texture_Source_Base - can we use a union or something to get texture and gradient on the same page?
	struct Texture_Source_Base : Asset
	{
		uint32_t gl_id = 0;
		float_t w = 0.f;
		float_t h = 0.f;

		virtual ~Texture_Source_Base() override;
	};
}
