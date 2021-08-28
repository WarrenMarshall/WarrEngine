
namespace war
{

struct Bit_Flag_Generator final
{
	uint16 bit = 1;

	Bit_Flag_Generator( uint16 start_bit );

	uint16 get();
	uint16 next();
};

}
