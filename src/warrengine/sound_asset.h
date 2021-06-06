
namespace war
{

struct sound_asset final : asset
{
	bool looped = false;

	sf::Sound sound;
	sf::SoundBuffer buffer;

	virtual ~sound_asset() override;

	void play();
	void pause();
	void stop();
	virtual bool create() override;
	virtual void adjust_for_time_dilation() override;
};

}