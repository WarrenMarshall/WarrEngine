
// ----------------------------------------------------------------------------

struct E_Player : Entity
{
	E_Player();

	[[nodiscard]] bool can_fire_weapon();
	void fire_weapon( i32 angle );

	Timer fire_cooldown;
};

// ----------------------------------------------------------------------------

struct E_Player_Bullet : Entity
{
	E_Player_Bullet();
};
