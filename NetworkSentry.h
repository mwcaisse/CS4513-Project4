/*
 * NetworkSentry.h
 *
 *  Created on: May 1, 2014
 *      Author: mitchell
 */

#ifndef NETWORKSENTRY_H_
#define NETWORKSENTRY_H_

#include "include/Object.h"

class NetworkSentry: public Object {


public:
	NetworkSentry();
	virtual ~NetworkSentry();

	/** Step event, checks the network manager to see if any messages are available,
	 * 		if there are messages available notifies everyone interested in network events
	 *
	 */

	void step();
};

#endif /* NETWORKSENTRY_H_ */
