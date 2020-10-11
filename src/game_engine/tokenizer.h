#pragma once

// ----------------------------------------------------------------------------

struct w_tokenizer
{
	std::string_view string_buffer;
	int idx = 0;
	char delim = 0;
	bool end_of_string = false;

	w_tokenizer( std::string_view string_buffer, char delim );
	~w_tokenizer() = default;

	std::optional<std::string_view> get_next_token();
	[[nodiscard]] bool is_eos();
};
