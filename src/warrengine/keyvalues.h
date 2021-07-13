
namespace war
{

struct key_values
{
	[[nodiscard]] bool does_key_exist( std::string_view key ) const;
	[[nodiscard]] std::string_view find_value( std::string_view key ) const;
	[[nodiscard]] std::string_view find_value_or( std::string_view key, std::string_view default_value = "" ) const;

	std::unordered_map<std::string, std::string> kv;
};

}
