/*
 * Host.h
 *
 *  Created on: May 1, 2014
 *      Author: mitchell
 */

#ifndef HOST_H_
#define HOST_H_

#include "include/Object.h"
#include "EventStep.h"
#include "EventNetwork.h"
#include "EventStep.h"

class ClientHero;

class Host: public Object {

private:

	/** The client hero that this host controls */
	ClientHero* clientHero;

	/** Handles network events
	 * 	@param event A pointer to the network event
	 */

	void networkHandle(EventNetwork* event);

	/** Handles step events
	 *  @param event A pointer to the step event
	 */

	void stepHandle(EventStep* event);

public:

	Host();

	virtual ~Host();

	/** Handles all events
	 */

	int eventHandler(Event* ev_p);

	/** Initializes the host for the gamestart
	 *
	 */

	void startGame();

};

#endif /* HOST_H_ */
