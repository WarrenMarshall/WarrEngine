
namespace war
{

struct game
{
	game( std::string internal_name );
	virtual ~game() = default;

	virtual void init();
	virtual void new_game();
	virtual void update();
	virtual void reset_scene_stack_to_main_menu();

	// the name of the data folder for the current game.
	std::string internal_name;

	struct
	{
		// if true, this game has a main menu and wants the "Main Menu" option
		// to appear on the ESC menu.
		bool has_main_menu : 1 = false;

	} flags;
};

}
