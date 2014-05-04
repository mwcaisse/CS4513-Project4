/*
 * NetworkStats.h
 *
 *  Created on: May 4, 2014
 *      Author: mitchell
 */

#ifndef NETWORKSTATS_H_
#define NETWORKSTATS_H_

#include "include/Object.h"

class EventNetwork;
class EventStep;

class NetworkStats: public Object {

private:

	/** The number of bytes transfered, used for per/sec calc */
	int numBytes;

	/** The total number of bytes transfered */
	int totalBytes;

	/** The total number of seconds elapsed */
	int totalSeconds;

	/** The total number of messages received */
	int totalMessagesRecv;

	/** The message id */
	int msgId;

	void networkEvent(EventNetwork* event);

	void stepEvent(EventStep* event);

	/** Returns the current time in micro seconds */

	int getCurrentTime();

	/** Sends a time message for RTT
	 *
	 */

	void sendTimeMessage();

	void forwardTimeMessage(EventNetwork* event);

public:

	NetworkStats();

	virtual ~NetworkStats();

	int eventHandler(Event* ev_p);


};

#endif /* NETWORKSTATS_H_ */
