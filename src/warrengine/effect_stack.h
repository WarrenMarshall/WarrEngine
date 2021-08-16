
namespace war
{

struct effect_stack
{
	std::vector<std::unique_ptr<effect>> stack;

	void update();

	template<typename T, typename ...Args>
	effect* add_effect( Args &&...args )
	{
		stack.push_back( std::make_unique<T>( std::forward<Args>( args )... ) );
		return stack.back().get();
	}
};

}
