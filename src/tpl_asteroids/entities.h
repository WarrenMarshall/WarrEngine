
using namespace war;

// ----------------------------------------------------------------------------

struct e_player_ship : entity
{
	e_player_ship();

	void fire();

	bool is_outside_playfield();

	virtual void update() override;
};

// ----------------------------------------------------------------------------

struct e_player : entity
{
	std::array<e_player_ship*, 9> satellite_ships = {};
	timer gun_timer;

	e_player();
	void fire();
	virtual void update() override;
};

// ----------------------------------------------------------------------------

struct e_player_bullet : entity
{
	e_player_bullet();

	virtual void update() override;
};
