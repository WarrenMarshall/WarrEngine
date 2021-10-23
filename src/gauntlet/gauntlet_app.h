
using namespace war;

struct Gauntlet_App final : Game
{
	Gauntlet_App();

	virtual void init() override;
	virtual void new_game() override;
	virtual void update() override;
};
