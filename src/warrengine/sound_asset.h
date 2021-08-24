
namespace war
{

struct sound_asset final : asset
{
	bool looped = false;

	sf::SoundBuffer buffer;
	sf::Sound sound;

	void play();
	void pause();
	void stop();
	virtual bool create() override;
	virtual void adjust_for_time_dilation() override;
};

}