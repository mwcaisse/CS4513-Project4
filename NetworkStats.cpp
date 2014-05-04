/*
 * NetworkStats.cpp
 *
 *  Created on: May 4, 2014
 *      Author: mitchell
 */

#include <sys/time.h>
#include <cstdlib>

#include "NetworkStats.h"
#include "NetworkManager.h"
#include "EventNetwork.h"
#include "EventStep.h"
#include "LogManager.h"

NetworkStats::NetworkStats() {
	numBytes = 0;
	totalBytes = 0;
	totalSeconds = 0;
	totalMessagesRecv = 0;
	msgId = random();

	LogManager::getInstance().writeLog("NetworkStats::NetworkStats(): MSG ID: %d", msgId);

	registerInterest(STEP_EVENT);
	NetworkManager::getInstance().registerInterest(this, NETWORK_EVENT);

}

NetworkStats::~NetworkStats() {
	LogManager& logManager = LogManager::getInstance();

	logManager.writeLog("Total Messages received %d", totalMessagesRecv);
	logManager.writeLog("Average Message Size: %f bytes", ((float) totalBytes / (float) totalMessagesRecv));

	logManager.writeLog("Total Bytes received %d bytes", totalBytes);
	logManager.writeLog("Average throughput: %f bytes/sec", ((float) totalBytes / (float) totalSeconds));
}


int NetworkStats::eventHandler(Event* ev_p) {
	if (ev_p->getType() == NETWORK_EVENT) {
		networkEvent((EventNetwork*) ev_p);
		return 1; // processed
	}
	else if (ev_p->getType() == STEP_EVENT) {
		stepEvent((EventStep*) ev_p);
		return 1;
	}

	return 0; // ignored
}

void NetworkStats::networkEvent(EventNetwork* event) {
	if (event->getOperation() == TIME) {
		if (event->getMiscInt() == msgId) {
			//this is our message
			int sentTime = atoi(event->getObjectType().c_str());

			int timeDiff = getCurrentTime() - sentTime;
			LogManager::getInstance().writeLog("NetworkStats::networkEvent(): recv time msg, RTT: %d", timeDiff);
		}
		else {
			//this message was sent by someone else, forward it back to other
			forwardTimeMessage(event);
		}
	}
	else {
		//dont count time messages as part of our measurements
		totalMessagesRecv++;
		numBytes += event->getTotalLength();
		totalBytes += event->getTotalLength();
		LogManager::getInstance().writeLog("NetworkStats::networkEvent(): Recv a message, size: %d", event->getTotalLength());
	}
}

void NetworkStats::stepEvent(EventStep* event) {
	if (event->getStepCount() % 30 == 0) {
		//a second
		totalSeconds ++;
		LogManager::getInstance().writeLog("NetworkStats::stepEvent(): Recv: %d bytes/sec", numBytes);
		numBytes = 0;
	}
}

int NetworkStats::getCurrentTime() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	int ms = 1000000 * tv.tv_sec + tv.tv_usec;
	return ms;
}

/** Sends a time message for RTT
 *
 */

void NetworkStats::sendTimeMessage() {
	int currentTime = getCurrentTime();
	if (NetworkManager::getInstance().sendMessage(TIME, toString(currentTime), msgId) == -1) {
		LogManager::getInstance().writeLog("NetworkStats::sendTimeMessage(): Failed to send time message");
	}
}

void NetworkStats::forwardTimeMessage(EventNetwork* event) {
	if (NetworkManager::getInstance().sendMessage(TIME, event->getObjectType(), event->getMiscInt()) == -1) {
		LogManager::getInstance().writeLog("NetworkStats::sendTimeMessage(): Failed to forward time message");
	}
}

