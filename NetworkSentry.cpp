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

