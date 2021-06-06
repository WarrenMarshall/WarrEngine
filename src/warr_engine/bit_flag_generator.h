
namespace war
{

struct bit_flag_generator
{
	uint16 bit = 1;

	bit_flag_generator( uint16 start_bit );
	void reset();
	[[nodiscard]] unsigned next();
};

}
