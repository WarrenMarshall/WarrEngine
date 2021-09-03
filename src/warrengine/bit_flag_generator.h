
namespace war
{

struct Bit_Flag_Generator
{
	uint16_t bit = 1;

	Bit_Flag_Generator( uint16_t start_bit );

	uint16_t get();
	uint16_t next();
};

}
