#pragma once

// ----------------------------------------------------------------------------

struct w_tokenizer
{
	std::string_view string_buffer = "";
	size_t idx = 0;
	char delim = 0;
	bool end_of_string = false;

	w_tokenizer() = default;
	w_tokenizer( std::string_view string_buffer, char delim );

	void init( std::string_view string_buffer, char delim );
	void preprocess();

	std::vector<std::string_view> tokens;

	std::optional<std::string_view> get_next_token();
	_NODISCARD bool is_eos();
};
