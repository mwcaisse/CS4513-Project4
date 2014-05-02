/*
 * HostStatus.h
 *
 *  Created on: May 1, 2014
 *      Author: mitchell
 */

#ifndef HOSTSTATUS_H_
#define HOSTSTATUS_H_

class HostStatus {

private:

	/** Whether or not we are host */
	static bool host;

	HostStatus();

	virtual ~HostStatus();


public:

	/** Returns if we are host or not
	 *
	 */

	static bool isHost();

	/** Sets if we are host or not
	 *
	 */

	static void setHost(bool host);
};

#endif /* HOSTSTATUS_H_ */
