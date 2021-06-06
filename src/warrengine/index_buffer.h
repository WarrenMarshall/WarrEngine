
namespace war
{

struct index_buffer
{
	index_buffer();
	~index_buffer();

	unsigned int gl_id;

	void bind();
	void unbind();
};

// ----------------------------------------------------------------------------

struct index_buffer_quads final : index_buffer
{
	index_buffer_quads();
};

// ----------------------------------------------------------------------------

struct index_buffer_tris final : index_buffer
{
	index_buffer_tris();
};

// ----------------------------------------------------------------------------

struct index_buffer_lines final : index_buffer
{
	index_buffer_lines();
};

// ----------------------------------------------------------------------------

struct index_buffer_points final : index_buffer
{
	index_buffer_points();
};

}
