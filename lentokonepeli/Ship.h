#pragma once
#include <SFML\Graphics.hpp>
#include "PhysicsTransformable.h"
#include "Damageable.h"
#include "Collider.h"
#include "Team.h"
#include "Weapon.h"

struct User;

class Ship : public PhysicsTransformable, public Collider<sf::RectangleShape>, public Damageable {
public:
	Ship() {}
	Ship(sf::Uint32 pTransId_, User* user_, TeamId teamId_ = NO_TEAM);

	void assignTeam(TeamId teamId_);

	bool hasTeam() const { return (teamId != NO_TEAM); }

	User* owner = nullptr;

	TeamId teamId;

	void takeDmg(float dmg) override;

	void restoreHealth(float heal) override;

	void onDeath() override;

	void draw(sf::RenderTarget& target);

	// heals to full and enables hitbox
	void respawn();

	void onCollision() override;
	void updateHitbox() override;
	void setWeaponTrans(sf::Vector2f pos, float rot);

	std::unique_ptr<Weapon> weapon;

	sf::Clock timeSinceDeath;

	bool localPlayer = false;

	bool throttle = false;

private:

	sf::Text usernameLabel;

	sf::RectangleShape rectangle;

	sf::RectangleShape healthBar;
	sf::RectangleShape healthBarBG;
	float hbMaxLength = 14.0F;
	float hbBorderSize = 0.5F;

	sf::Clock dmgTimer;
	float dmgTime = 0.05F;
};