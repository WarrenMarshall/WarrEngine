
namespace war
{
	struct Texture_Source_Base : Asset
	{
		ui32 gl_id = 0;
		f32 w = 0.f;
		f32 h = 0.f;

		virtual ~Texture_Source_Base() override;
	};
}
