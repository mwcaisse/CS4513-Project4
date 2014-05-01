/*
 * Host.h
 *
 *  Created on: May 1, 2014
 *      Author: mitchell
 */

#ifndef HOST_H_
#define HOST_H_

#include "include/Object.h"


class Host: public Object {

public:

	Host();

	virtual ~Host();

	/** Handles all events
	 */

	int eventHandler(Event* ev_p);

};

#endif /* HOST_H_ */
