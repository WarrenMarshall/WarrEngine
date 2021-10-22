
namespace war
{

struct Index_Buffer
{
	Index_Buffer();
	~Index_Buffer();

	ui32 gl_id;

	void bind();
	void unbind();
};

// ----------------------------------------------------------------------------

struct Index_Buffer_Quads final : Index_Buffer
{
	Index_Buffer_Quads();
};

// ----------------------------------------------------------------------------

struct Index_Buffer_Tris final : Index_Buffer
{
	Index_Buffer_Tris();
};

// ----------------------------------------------------------------------------

struct Index_Buffer_Lines final : Index_Buffer
{
	Index_Buffer_Lines();
};

// ----------------------------------------------------------------------------

struct Index_Buffer_Points final : Index_Buffer
{
	Index_Buffer_Points();
};

}
