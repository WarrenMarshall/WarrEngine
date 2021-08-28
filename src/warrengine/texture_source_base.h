
namespace war
{
	// #Texture_Source_Base - can we use a union or something to get texture and gradient on the same page?
	struct Texture_Source_Base : Asset
	{
		unsigned int gl_id = 0;
		float w = 0.f;
		float h = 0.f;

		virtual ~Texture_Source_Base() override;
	};
}
