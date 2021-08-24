
namespace war
{
	struct texture_source_base : asset
	{
		unsigned int gl_id = 0;
		float w = 0.f;
		float h = 0.f;

		virtual ~texture_source_base() override;
	};
}
