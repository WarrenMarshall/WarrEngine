
namespace war
{

struct Cmdline_Args final
{
	// "-verbose" to set TRUE
	//		* enables optional log messages that are marked using "log_verbose"
	bool verbose = false;

	// "-nobatch" to set TRUE
	//		* pretend like the video card has a single texture unit
	bool nobatch = false;

	// "-force_vsync" to set TRUE
	//		* forces the GPU to use vsync
	bool force_vsync = false;

	// "-developer" to set TRUE
	//		* allows the display of stats and debug info

#ifdef _DEBUG
	bool developer = true;
#else
	bool developer = false;
#endif
};

}
