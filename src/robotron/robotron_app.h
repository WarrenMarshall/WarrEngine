
using namespace war;

struct Robotron_App final : Game
{
	Robotron_App();

	virtual void init() override;
	virtual void new_game() override;
	virtual void update() override;
};
