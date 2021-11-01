
// ----------------------------------------------------------------------------

struct Player_Entity : Entity
{
	Player_Entity();

	[[nodiscard]] bool can_fire_weapon();
	void fire_weapon( i32 angle );

	Timer fire_cooldown;
	Sprite_Component* sprite_component = nullptr;
	Sound_Asset* snd_player_shoot = nullptr;

	virtual void pre_update() override;
};

// ----------------------------------------------------------------------------

struct Player_Bullet_Entity : Entity
{
	Player_Bullet_Entity();
};

// ----------------------------------------------------------------------------

struct Enemy_Entity : Entity
{
	// the entity that this enemy is targetting. this can mean moving towards or
	// shooting at, depending on what this enemy can do.
	Entity* target = nullptr;

	Timer next_move_timer;
	Timer next_find_new_target_timer;

};

struct Enemy_Drone_Entity : Enemy_Entity
{
	Enemy_Drone_Entity();

	virtual void update() override;
private:
	void find_new_target();
	void move_towards_target();
};
