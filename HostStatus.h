/*
 * HostStatus.h
 *
 *  Created on: May 1, 2014
 *      Author: mitchell
 */

#ifndef HOSTSTATUS_H_
#define HOSTSTATUS_H_

class Client;
class Host;

class HostStatus {

private:

	/** Whether or not we are host */
	static bool bHost;

	static Host* host;

	static Client* client;

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

	static Host* getHost();

	static void setHost(Host* host);

	static Client* getClient();

	static void setClient(Client* client);
};

#endif /* HOSTSTATUS_H_ */
