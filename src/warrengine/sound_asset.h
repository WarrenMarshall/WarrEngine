
namespace war
{

struct Sound_Asset final : Asset
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