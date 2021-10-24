
struct Scene_Gameplay final : Scene
{
	E_Player* player = nullptr;

	Scene_Gameplay();

	virtual void pushed() override;
	virtual void draw() override;
	virtual bool on_input_pressed( const Input_Event* evt ) override;
};
