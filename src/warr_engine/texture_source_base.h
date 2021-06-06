
namespace war
{
	struct texture_source_base : asset
	{
		unsigned int gl_id = 0;
		float w = 0.f;
		float h = 0.f;

		~texture_source_base();
	};
}
