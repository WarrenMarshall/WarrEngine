
namespace war
{

struct Index_Buffer
{
	Index_Buffer();
	~Index_Buffer();

	uint32_t gl_id;

	void bind();
	void unbind();
};

// ----------------------------------------------------------------------------

struct Index_Buffer_Quads : Index_Buffer
{
	Index_Buffer_Quads();
};

// ----------------------------------------------------------------------------

struct Index_Buffer_Tris : Index_Buffer
{
	Index_Buffer_Tris();
};

// ----------------------------------------------------------------------------

struct Index_Buffer_Lines : Index_Buffer
{
	Index_Buffer_Lines();
};

// ----------------------------------------------------------------------------

struct Index_Buffer_Points : Index_Buffer
{
	Index_Buffer_Points();
};

}
