#pragma once

// ----------------------------------------------------------------------------

struct GE_API w_tokenizer
{
	const std::string_view string_buffer;
	char delim = 0;
	std::string def_value = "";
	bool end_of_string = false;

	int idx = 0;

	w_tokenizer( const std::string_view string_buffer, char delim, const std::string& def_value = "" );

	std::string_view get_next_token();

	bool is_eos() { return end_of_string; }
};
