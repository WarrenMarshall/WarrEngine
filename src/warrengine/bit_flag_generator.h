
namespace war
{

struct bit_flag_generator
{
	uint16 bit = 1;

	bit_flag_generator( uint16 start_bit );

	uint16 operator++();
	uint16 operator++( int );
};

}
