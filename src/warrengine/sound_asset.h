
namespace war
{

struct Sound_Asset final : Asset
{
	struct
	{
		bool looped : 1 = false;

		// if true, the pitch of the sound will have a random value added to it
		// so it doesn't sound exactly the same every time we play it.
		bool vary_pitch : 1 = false;
	} flags;

	// the range of pitch values that get added to the sound if "flags.vary_pitch" is true.
	Range pitch_variance = Range( -.25f, .25f );

	sf::SoundBuffer buffer;
	sf::Sound sound;

	void play();
	void pause();
	void stop();
	virtual bool create() override;
	virtual void adjust_for_time_dilation() override;
};

}