
namespace war
{

struct game
{
	game( std::string name );
	virtual ~game() = default;

	virtual void init();
	virtual void new_game();
	virtual void update();
	virtual void reset_scene_stack_to_main_menu();

	const char* get_game_name();

	std::string name;

	struct
	{
		// if true, this game has a main menu and wants the "Main Menu" option
		// to appear on the ESC menu.
		bool has_main_menu = false;

		// if true, this game requires a controller to play. if a controller is
		// not connected, the "controller required" scene will be displayed
		// automatically whenever the engine can't find a connected controller.
		bool requires_controller = false;

	} flags;
};

}
