
namespace war
{

// ----------------------------------------------------------------------------
// a file that has been read into memory, either from the hard drive or a ZIP
// file

struct file_mem
{
	std::vector<char> buffer;

	file_mem() = default;
	file_mem( int size );
};

// ----------------------------------------------------------------------------
// same as w_file_mem except it adds a preprocess step after loading that strips
// out stuff that is only useful to humans - comments, line continuations, blank
// lines, etc.

struct file_mem_text
{
	std::vector<char> buffer;
	std::vector<std::string> lines;

	file_mem_text() = default;
	file_mem_text( int size );

	void preprocess();
	void preprocess_raw();
};

}
