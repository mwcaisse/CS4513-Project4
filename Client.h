/*
 * Client.h
 *
 *  Created on: May 1, 2014
 *      Author: mitchell
 */

#ifndef CLIENT_H_
#define CLIENT_H_

#include "include/Object.h"

#include "EventNetwork.h"
#include "EventStep.h"
#include "EventKeyboard.h"



class Client: public Object {

private:

	/** Handles network events
	 *	@param event A pointer to the network event
	 */

	void networkHandle(EventNetwork* event);

	/** Handles keyboard events
	 * 	@param event A pointer to the keyboard event
	 */

	void keyboardHandle(EventKeyboard* event);

	/** Handles step events
	 *  @param event A pointer to the step event
	 */

	void stepHandle(EventStep* event);

	void createObject(EventNetwork* event);

	void updateObject(EventNetwork* event);

	void deleteObject(EventNetwork* event);

	void updatePoints(EventNetwork* event);

	void gameOver(EventNetwork* event);

public:

	Client();

	virtual ~Client();

	/** Handles all events
	 *
	 */

	int eventHandler(Event* ev_p);

};

#endif /* CLIENT_H_ */
