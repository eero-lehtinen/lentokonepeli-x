#include "ServerGame.h"
#include "Master.h"
#include <iostream>

ServerGame::ServerGame(Master* master_) : Game(master_) {
	server.init(master_, this);
	server.start(4);
}

void ServerGame::loop() {
	float dt = 1.0F / fixedUpdateFps;

	sf::Clock clock;

	int renderCounter = 0;

	while (master->window.isOpen() && running) {

		sf::Event event;
		while (master->window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				master->quit();
			}
			gui->handleEvent(event);
		}
		
		float frameTime = clock.restart().asSeconds();
		updateFrameTimes(frameTime);

		fixedUpdate(dt);

		renderCounter++;
		if (renderCounter == 2) {
			render(master->window);
			renderCounter = 0;
		}

		sf::sleep(sf::seconds(dt - clock.getElapsedTime().asSeconds()));
	}
}

void ServerGame::onUserConnect(User* const user) {
	goManager.createShip(user);
}

void ServerGame::onUserDisconnect(sf::Uint8 clientId) {
	goManager.removeShip(clientId);
}

void ServerGame::onClientJoinTeam(sf::Uint8 clientId, TeamId newTeam) {
	if (newTeam != NO_TEAM)
		server.sendAllowSpawnMsg(clientId);
}

void ServerGame::render(sf::RenderWindow& window) {
	window.clear(sf::Color(62, 65, 71));

	gui->draw();

	window.display();
}

void ServerGame::fixedUpdate(float dt) {
	goManager.previousPTransformsState = goManager.currentPTransformsState;
	
	server.update();

	ServerShipStates serverStates;
	
	// TODO: integrate ships when we don't receive a movement packet

	for (auto& pair : server.shipStateJitterBuffers) {
		if (pair.second.size() > 0) {
			serverStates.states[pair.first] = pair.second.front();
			pair.second.pop_front();
		}
	}

	applyServerStates(serverStates);

	// TODO: integrate non player controlled objects, eg. bullets

	goManager.applyTransforms(goManager.currentPTransformsState);

	// TODO: Check collisions (bullet hits)

	updateServerStates(serverStates);


	// TODO: and if dead then send time until respawn to client

	server.broadcastShipStates(serverStates);
}

void ServerGame::applyServerStates(ServerShipStates& sss) {
	for (auto& pair : sss.states) {
		
		ShipState& shipState = pair.second;
		sf::Uint8 clientId = pair.first;

		if (server.users.count(clientId) == 1) {

			shipState.applyToPTrans(goManager.currentPTransformsState.at(goManager.ships.at(clientId).pTransId));

			// TODO: check respawning timer
			if (goManager.ships.at(clientId).isDead() == true && shipState.dead == false) {
				goManager.ships.at(clientId).setHealthToFull();
			}

			if (shipState.shoot) {
				goManager.ships.at(clientId).weapon->shoot(shipState.bulletId, false);
			}
		}
	}
}

void ServerGame::updateServerStates(ServerShipStates& sss) {
	for (auto& pair : goManager.ships) {
		sss.states[pair.first].generateFromPTrans(pair.second);
		sss.states[pair.first].dead = pair.second.isDead();
	}
}



