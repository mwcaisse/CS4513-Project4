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

	/** Handles the network events
	 *
	 */

	int eventHandler(Event* event);

};

#endif /* HOST_H_ */
