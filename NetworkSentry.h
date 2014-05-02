/*
 * NetworkSentry.h
 *
 *  Created on: May 1, 2014
 *      Author: mitchell
 */

#ifndef NETWORKSENTRY_H_
#define NETWORKSENTRY_H_

#include "include/Object.h"

#define NETWORK_SENTRY_ID (50001)

class NetworkSentry: public Object {


public:
	NetworkSentry();
	virtual ~NetworkSentry();

	/** Handles all events
	 */

	int eventHandler(Event* ev_p);


	/** Step event, checks the network manager to see if any messages are available,
	 * 		if there are messages available notifies everyone interested in network events
	 *
	 */

	void step();
};

#endif /* NETWORKSENTRY_H_ */
