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

NetworkSentry::NetworkSentry() {
	registerInterest(STEP_EVENT);

}

NetworkSentry::~NetworkSentry() {
	unregisterInterest(STEP_EVENT);
}

/** Step event, checks the network manager to see if any messages are available,
 * 		if there are messages available notifies everyone interested in network events
 *
 */

void NetworkSentry::step() {
	NetworkManager& manager = NetworkManager::getInstance();

	//get all of the messages from the network manager
	while (manager.isMessage()) {
		//we have a message, retrieve the message
		EventNetwork* event = manager.recvMessage();
		if (event) {
			//notify all interested
			manager.onEvent(event);
		}
	}

}

