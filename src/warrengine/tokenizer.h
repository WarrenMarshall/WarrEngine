
namespace war
{

struct tokenizer
{
	std::string_view string_buffer = "";
	size_t idx = 0;
	std::string_view delim;
	bool end_of_string = false;
	bool keep_quoted_strings = false;

	tokenizer() = default;
	tokenizer( std::string_view string_buffer, std::string_view delim, bool keep_quoted_strings = false );

	void init( std::string_view string_buffer, std::string_view delim );
	void preprocess();

	std::deque<std::string_view> tokens;

	std::optional<std::string_view> get_next_token();
	[[nodiscard]] bool is_eos();
};

}