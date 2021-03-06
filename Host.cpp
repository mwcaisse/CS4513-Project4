/*
 * Host.cpp
 *
 *  Created on: May 1, 2014
 *      Author: mitchell
 */

#include "Host.h"

#include "LogManager.h"
#include "NetworkManager.h"
#include "WorldManager.h"
#include "EventNetwork.h"
#include "EventStep.h"
#include "ClientHero.h"
#include "ObjectListIterator.h"
#include "StaticIds.h"
#include "GameStart.h"
#include "GameManager.h"

Host::Host() {
	NetworkManager::getInstance().registerInterest(this, NETWORK_EVENT);
	registerInterest(STEP_EVENT);

}

Host::~Host() {
	NetworkManager::getInstance().unregisterInterest(this, NETWORK_EVENT);
	unregisterInterest(STEP_EVENT);
}

/** Handles the registered events, network, and step
 *	@return 0 if ignored, 1 otherwise
 */

int Host::eventHandler(Event* ev_p) {
	if (ev_p->getType() == NETWORK_EVENT) {
		networkHandle((EventNetwork*) ev_p);
		return 1; // processed
	}
	else if (ev_p->getType() == STEP_EVENT) {
		stepHandle((EventStep*) ev_p);
		return 1;
	}

	return 0; // ignored
}

/** Handles network events
 * 	@param event A pointer to the network event
 */

void Host::networkHandle(EventNetwork* event) {
	if (event->getOperation() ==  KEYSTK) {
		//this was a keystroke, otherwise host doesn't do anything
		int key = event->getMiscInt();

		//LogManager &log_manager = LogManager::getInstance();
		//log_manager.writeLog("Host::networkHandle: received a keystroke from the client: %d", key);
		if (clientHero) {
			clientHero->keyboard(key);
		}

	}
	else if (event->getOperation() == GAME_START) {
		Object* obj = WorldManager::getInstance().objectWithId(GAME_START_ID);
		GameStart* gameStartObj = static_cast<GameStart*>(obj);
		gameStartObj->startOther();
	}
	else if (event->getOperation() == QUIT) {
		GameManager::getInstance().setGameOver();
	}
}

/** Handles step events
 *  @param event A pointer to the step event
 */

void Host::stepHandle(EventStep* event) {
	if (event->getStepCount() % 30 == 0) {

	}
}

/** Initializes the host for the gamestart
 *
 */

void Host::startGame() {
	clientHero = new ClientHero();
}

void Host::clientHeroDeleted() {
	clientHero = NULL;
}

