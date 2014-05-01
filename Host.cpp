/*
 * Host.cpp
 *
 *  Created on: May 1, 2014
 *      Author: mitchell
 */

#include "Host.h"

#include "NetworkManager.h"
#include "EventNetwork.h"
#include "EventStep.h"

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

		//cast it into a network event
		EventNetwork* event = (EventNetwork) ev_p;

		if (event->getOperation() ==  KEYSTK) {
			//this was a keystroke, otherwise host doesn't do anything
			int key = event->getMiscInt();

			//TODO: send this to our client host.

		}

		return 1; // processed
	}
	else if (ev_p->getType() == STEP_EVENT) {
		//we have a step event
		//check all synchronizable game objects.

		return 1;
	}

	return 0; // ignored
}

