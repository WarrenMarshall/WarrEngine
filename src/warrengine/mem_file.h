
namespace war
{

// ----------------------------------------------------------------------------
// a file that has been read into memory, either from the hard drive or a ZIP
// file

struct Mem_File
{
	std::vector<char> buffer;

	Mem_File() = default;
	Mem_File( int32_t size );
};

// ----------------------------------------------------------------------------
// same as w_file_mem except it adds a preprocess step after loading that strips
// out stuff that is only useful to humans - comments, line continuations, blank
// lines, etc.

struct Mem_File_Text
{
	std::vector<char> buffer;
	std::vector<std::string> lines;

	Mem_File_Text() = default;
	Mem_File_Text( int32_t size );

	void preprocess();
	void preprocess_raw();
};

}
