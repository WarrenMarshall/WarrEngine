
using namespace war;

#define my_game ((tpl_asteroids_game*)g_base_game.get())

// ----------------------------------------------------------------------------

struct player_ship_geo
{
	std::vector<vec2> verts;
};

// ----------------------------------------------------------------------------

constexpr int max_ship_power_level = 11;

struct tpl_asteroids_game : game
{
	static std::string get_custom_name()
	{
		return "tpl_asteroids";
	}

	tpl_asteroids_game( std::string name );

	virtual void init() override;
	virtual void new_game() override;
	virtual void update() override;

	// the shapes of the player ship at different power levels
	std::array<player_ship_geo, max_ship_power_level> ship_geo;

	int ship_power_level = 0;
	int player_score = 12357;
	float ship_radius = 12.f;
	float bullet_base_movement_speed = 100.f;
	float bullet_per_level_movement_speed = 7.f;
	float ship_base_rotation_speed = 20.f;
	float ship_per_level_rotation_speed = 5.f;
	float ship_base_movement_speed = 75.f;

	time_ms get_ship_powerup_timer_duration();
	void level_up_player_ship();
	bool ship_is_at_max_level();
};
