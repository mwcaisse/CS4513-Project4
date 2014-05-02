/*
 * NetworkSentry.cpp
 *
 *  Created on: May 1, 2014
 *      Author: mitchell
 */

#include "NetworkSentry.h"
#include "NetworkManager.h"
#include "EventNetwork.h"
#include "EventStep.h"
#include "LogManager.h"

NetworkSentry::NetworkSentry() {
	registerInterest(STEP_EVENT);

}

NetworkSentry::~NetworkSentry() {
	unregisterInterest(STEP_EVENT);
}

/** OnEvent handler
 *  @param ev_p A pointer to the event object
 */

int NetworkSentry::eventHandler(Event* ev_p) {
	if (ev_p->getType() == STEP_EVENT) {
		step();
		return 1;
	}
	return 0;
}

/** Step event, checks the network manager to see if any messages are available,
 * 		if there are messages available notifies everyone interested in network events
 *
 */

void NetworkSentry::step() {
	LogManager &logManager = LogManager::getInstance();
	NetworkManager& manager = NetworkManager::getInstance();

	//int data = manager.isData();
	//get all of the messages from the network manager
	if (manager.isMessage()) {
		//we have a message, retrieve the message
		EventNetwork* event = manager.recvMessage();
		if (event) {
			//notify all interested
			manager.onEvent(event);
		}
	}

}

