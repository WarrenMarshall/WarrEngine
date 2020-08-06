#pragma once

// ----------------------------------------------------------------------------
// a file that has been read into memory, either from the
// hard drive or a ZIP file

struct w_mem_file
{
	bool was_loaded_from_zip_file = false;
	std::unique_ptr<std::vector<char>> buffer = nullptr;

	w_mem_file();
};

// ----------------------------------------------------------------------------
// adds a preprocess step after loading that strips out stuff
// that is only useful to humans - comments, line continuations, blank, etc.

struct w_mem_file_text : w_mem_file
{
	std::unique_ptr<std::vector<std::string>> lines = nullptr;

	w_mem_file_text();
	void preprocess();
};
