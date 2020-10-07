#pragma once

// ----------------------------------------------------------------------------

struct w_tokenizer
{
	const std::string_view string_buffer;
	char delim = 0;
	std::string def_value;
	bool end_of_string = false;

	int idx = 0;

	w_tokenizer( const std::string_view string_buffer, char delim, std::optional<std::string> def_value = std::nullopt );

	std::optional<std::string_view> get_next_token();
	[[nodiscard]] bool is_eos();
};
