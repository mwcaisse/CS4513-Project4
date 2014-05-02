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

		LogManager &log_manager = LogManager::getInstance();
		log_manager.writeLog("Host::networkHandle: received a keystroke from the client: %d", key);
		clientHero->keyboard(key);
	}
}

/** Handles step events
 *  @param event A pointer to the step event
 */

void Host::stepHandle(EventStep* event) {
	/*WorldManager& worldManager = WorldManager::getInstance();
	NetworkManager& networkManager = NetworkManager::getInstance();

	ObjectList objectList = worldManager.getAllObjects();
	ObjectListIterator* objectItr = new ObjectListIterator(&objectList);

	objectItr->first(); // set it to the first object

	while (!objectItr->isDone()) {

		Object* obj = objectItr->currentObject();

		//if the object was modified, and it is a bullet or saucer
		if ( obj->isModified() && (obj->getType() == "Bullet" || obj->getType() == "Saucer")) {
			networkManager.sendUpdateMessage(obj);
		}

		objectItr->next();
	}*/


}

/** Initializes the host for the gamestart
 *
 */

void Host::startGame() {
	clientHero = new ClientHero();
}

