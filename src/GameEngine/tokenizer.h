#pragma once

// ----------------------------------------------------------------------------

struct w_tokenizer
{
	const std::string& string_buffer = nullptr;
	char delim = 0;
	std::string def_value = "";
	bool end_of_string = false;

	int idx = 0;

	w_tokenizer( const std::string& string_buffer, char delim, std::string def_value = "" );

	std::string get_next_token();

	bool is_eos() { return end_of_string; }
};
