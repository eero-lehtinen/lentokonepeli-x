#pragma once

#include "RakNet/WindowsIncludes.h"
#include <SFML\System.hpp>
#include <unordered_map>
#include "GOManager.h"
#include "FpsCounter.h"
#include "Closeable.h"
#include "TestLevel.h"

struct Input {
	bool moveForward = false;
	bool turnLeft = false;
	bool turnRight = false;
	bool shooting = false;
	bool abilityForward = false;
	bool abilityLeft = false;
	bool abilityBackward = false;
	bool abilityRight = false;

	bool any() { return (moveForward || turnLeft || turnRight || shooting || abilityForward || abilityLeft || abilityBackward || abilityRight); }
};

struct InputResponse {
	int bulletId = -1;
	bool isUsingAbility = false;
};

class Game : public FpsCounter, public Closeable {
public:
	Game() { goManager.init(this); }

	virtual void loop() = 0;

	virtual void onShipDeath(Ship* ship) = 0;

	GOManager goManager;

protected:
	virtual void spawnShip(sf::Uint8 clientId) = 0;

	const float fixedUpdateFps = 60.0F;
	const sf::Vector2f gravity{0, 10.0F};

	void integrate(PhysicsTransformable& pTrans, float dt);

	// Call when player presses throttle
	void improveHandling(Ship& ship);

	void collisionDetectAll(std::unordered_map<Team::Id, Team>& teams);

	virtual void onBulletCollision(Bullet& bullet, Ship& targetShip) = 0;
	virtual void onShipCollision(Ship& s1, Ship& s2) = 0;

	void quit();

	TestLevel level;

	std::unordered_map<sf::Uint8, CountdownTimer> spawnTimers;

	Scoreboard scoreBoard;
	
	// checks timers and spawns ships if timer allows
	virtual void handleSpawnTimers(float dt);
};

