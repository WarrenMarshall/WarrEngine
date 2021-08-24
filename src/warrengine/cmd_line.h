
namespace war
{

struct cmdline_args final
{
	// "-verbose" to set TRUE
	//		* enables optional log messages that are marked using "log_verbose"
	bool verbose = false;

	// "-nobatch" to set TRUE
	//		* pretend like the video card has a single texture unit
	bool nobatch = false;
};

}
