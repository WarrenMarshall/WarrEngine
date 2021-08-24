
namespace war
{

struct key_values final
{
	[[nodiscard]] bool does_key_exist( const std::string& key ) const;
	[[nodiscard]] std::string_view find_value( const std::string& ) const;
	[[nodiscard]] std::string_view find_value_or( const std::string&, const std::string& default_value = "" ) const;

	std::map<std::string, std::string> kv;
};

}
