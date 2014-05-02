/*
 * Client.cpp
 *
 *  Created on: May 1, 2014
 *      Author: mitchell
 */

#include "Client.h"
#include "NetworkManager.h"
#include "EventNetwork.h"
#include "EventStep.h"
#include "EventKeyboard.h"
#include "LogManager.h"
#include "Bullet.h"
#include "Saucer.h"
#include "WorldManager.h"

Client::Client() {
	NetworkManager::getInstance().registerInterest(this, NETWORK_EVENT);
	registerInterest(STEP_EVENT);
	registerInterest(KEYBOARD_EVENT);
}

Client::~Client() {
	NetworkManager::getInstance().unregisterInterest(this, NETWORK_EVENT);
	unregisterInterest(STEP_EVENT);
	unregisterInterest(KEYBOARD_EVENT);
}

/** Handles all events
 *
 */

int Client::eventHandler(Event* ev_p) {
	if (ev_p->getType() == NETWORK_EVENT) {
		networkHandle( (EventNetwork*) ev_p);
		return 1;
	}
	else if (ev_p->getType() == KEYBOARD_EVENT) {
		keyboardHandle( (EventKeyboard*) ev_p);
		return 1;
	}
	else if (ev_p->getType() == STEP_EVENT) {
		stepHandle( (EventStep*) ev_p);
		return 1;
	}
	return 0;
}

/** Handles network events
 *	@param event A pointer to the network event
 */

void Client::networkHandle(EventNetwork* event) {

	//find out what event it was, client shouldn't get keystroke events
	switch (event->getOperation()) {

	case CREATE:
		createObject(event);
		break;
	case UPDATE:
		updateObject(event);
		break;
	case DELETE:
		deleteObject(event);
		break;

	}

}

void Client::createObject(EventNetwork* event) {
	if (event->getObjectType() == "Bullet") {
		new Bullet(event->getData());
	}
	else if (event->getObjectType() == "Saucer") {
		new Saucer(event->getData());
	}
}

void Client::updateObject(EventNetwork* event) {
	int id = event->getMiscInt();
	WorldManager& worldManager = WorldManager::getInstance();
	Object* toUpdate = worldManager.objectWithId(id);
	toUpdate->deserialize(event->getData());
}

void Client::deleteObject(EventNetwork* event) {
	int id = event->getMiscInt();
	WorldManager& worldManager = WorldManager::getInstance();
	Object* toDelete = worldManager.objectWithId(id);
	worldManager.markForDelete(toDelete);
}

/** Handles keyboard events
 * 	@param event A pointer to the keyboard event
 */

void Client::keyboardHandle(EventKeyboard* event) {
	LogManager &logManager = LogManager::getInstance();

	//get the keyboard key pressed
	int key = event->getKey();

	//send the key event to the host
	NetworkManager& networkManager = NetworkManager::getInstance();
	int res = networkManager.sendMessage(KEYSTK, "", key);

	//check for success
	if (res < 0) {
		logManager.writeLog("Client::keyboardHandle: Warning! Could not send key stroke to host");
	}

}

/** Handles step events
 *  @param event A pointer to the step event
 */

void Client::stepHandle(EventStep* event) {

}
