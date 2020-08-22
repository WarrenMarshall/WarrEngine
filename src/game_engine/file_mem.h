#pragma once

// ----------------------------------------------------------------------------
// a file that has been read into memory, either from the
// hard drive or a ZIP file

struct w_file_mem
{
	std::unique_ptr<std::vector<char>> buffer = nullptr;
	struct
	{
		unsigned char was_loaded_from_zip_file : 1 = false;
	};

	w_file_mem() = default;
	w_file_mem( int size );
};

// ----------------------------------------------------------------------------
// adds a preprocess step after loading that strips out stuff
// that is only useful to humans - comments, line continuations, blank, etc.

struct w_mem_file_text : w_file_mem
{
	std::unique_ptr<std::vector<std::string>> lines = nullptr;

	w_mem_file_text() = default;
	w_mem_file_text( int size);
	void preprocess();
};
