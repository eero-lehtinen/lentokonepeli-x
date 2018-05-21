#include "Game.h"
#include <Thor/Vectors.hpp>
#include <iostream>
#include "PhysicsTransformable.h"

void Game::integrate(PhysicsTransformable& currPTrans, float dt){
	if (currPTrans.constantVelocity == false) {
		sf::Vector2f force;
		if (currPTrans.gravity)
			force += gravity;

		force += currPTrans.forceOnSelf; // Internal force eg. ship engine

		if (thor::squaredLength(currPTrans.velocity) != 0.0F)
			force += thor::squaredLength(currPTrans.velocity) * currPTrans.drag * -thor::unitVector(currPTrans.velocity); // drag

		currPTrans.acceleration = force; // f/m when m = 1
		currPTrans.velocity += currPTrans.acceleration * dt;

		float angularForce = 0;
	}
	currPTrans.move(currPTrans.velocity * dt); 
	currPTrans.rotate(currPTrans.angularVelocity * dt);
}

void Game::improveHandling(Ship& ship) {
	PhysicsTransformable& target = goManager.currentPTransformsState[ship.pTransId];

	float velocityMagnitude = thor::length(target.velocity);

	if (velocityMagnitude != 0) {

		float angle = thor::signedAngle(target.getRotationVector(), target.velocity);

		float factor = (-0.00012F * angle * angle + 1.0F) * 0.08F;
		if (factor < 0.0F)
			factor = 0.0F;

		target.velocity = target.getRotationVector() * velocityMagnitude * factor + target.velocity * (1.0F - factor);
	}
}


void Game::quit() {
	running = false;
}