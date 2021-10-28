
// ----------------------------------------------------------------------------

struct E_Player : Entity
{
	E_Player();

	[[nodiscard]] bool can_fire_weapon();
	void fire_weapon( i32 angle );

	Timer fire_cooldown;
	Sprite_Component* sprite_component = nullptr;
	Sound_Asset* snd_player_shoot = nullptr;

	virtual void pre_update() override;
};

// ----------------------------------------------------------------------------

struct E_Player_Bullet : Entity
{
	E_Player_Bullet();
};

// ----------------------------------------------------------------------------

struct E_Enemy_Drone : Entity
{
	E_Enemy_Drone();
};
