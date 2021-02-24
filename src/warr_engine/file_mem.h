#pragma once

// ----------------------------------------------------------------------------
// a file that has been read into memory, either from the hard drive or a ZIP
// file

struct w_file_mem
{
	std::unique_ptr<std::vector<char>> buffer = nullptr;

	w_file_mem() = default;
	w_file_mem( int size );
};

// ----------------------------------------------------------------------------
// same as w_file_mem except it adds a preprocess step after loading that strips
// out stuff that is only useful to humans - comments, line continuations, blank
// lines, etc.

struct w_file_mem_text
{
	std::unique_ptr<std::vector<char>> buffer = nullptr;
	std::unique_ptr<std::vector<std::string>> lines = nullptr;

	w_file_mem_text() = default;
	w_file_mem_text( int size);

	void preprocess();
};
