#pragma once
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <deque>

class Master;

class SoundPlayer {
public:
	SoundPlayer() {}

	void init(Master* master);

	void playThrottle(const sf::Vector2f& position, int id);
	void stopThrottle(int id);

	void updateListenerPos(const sf::Vector2f& position);

	void playSound(const sf::Vector2f& position, std::string name);
private:

	std::string path = "res/sound/";

	std::unordered_map<std::string, sf::SoundBuffer> buffers;
	std::unordered_map<int, sf::Sound> throttleSounds;

	//Contans sounds that don't need identification ( no need to access specific ones after creation)
	std::deque<sf::Sound> sounds;

	void deleteOldSounds();
	
	// If a sound is close enough, just make it sound like it's inside your head
	void applySpacializationCutoff(sf::Sound& sound);
};