/*
 * NetworkManager.h
 *
 *  Created on: Apr 30, 2014
 *      Author: mitchell
 */

#ifndef NETWORKMANAGER_H_
#define NETWORKMANAGER_H_

#include "Manager.h"

#define DRAGONFLY_PORT "9876"

#define OBJECT_TYPE_LEN 100

#define MSG_OP_CREATE 1
#define MSG_OP_UPDATE 2
#define MSG_OP_DELETE 3
#define MSG_OP_KEYSTK 4


struct _network_message_header {
	int op; // The operation this message is performing
	int len; // the length of the message
	char object_type[OBJECT_TYPE_LEN]; // the object type
};

typedef struct _network_message_header message_header;


class NetworkManager : public Manager {


private:

	NetworkManager();

	NetworkManager(NetworkManager const&);

	void operator=(NetworkManager const&); // dont allow assignment

	/** The socket descriptor for the socket */
	int sock;

public:

	/** Returns the singleton instance of the NetworkManager */
	static NetworkManager &getInstance();

	/** Starts the network manager
	 *
	 */

	int startUp();

	/** Shuts down the network manager
	 *
	 */

	void shutDown();

	/** Checks to make sure the event type is valid
	 *
	 */

	bool isValid(std::string event_type);

	/** Listens for connections from the client
	 *
	 *	@param port The port to listen on
	 */

	int accept(std::string port = DRAGONFLY_PORT);

	/** Connects to the host at the specified address and port
	 *
	 * @param host The address of the host
	 * @param port The port to connect to
	 */

	int connect(std::string host, std::string port = DRAGONFLY_PORT);

	/** Close the network connection
	 *
	 */

	int close();

	/** Sends the specified bytes over the connected network
	 *
	 * @param buffer The bytes to send
	 * @param bytes The number of bytes to send
	 * @return The number of bytes sent or -1 if error occurred
	 *
	 */

	int send(void* buffer, int bytes);

	/** Reads up to the specified number of bytes into the buffer
	 *
	 * @param buffer The buffer to copy the data into
	 * @param bytes The maximum number of bytes to read
	 * @return The number of bytes read, -1 if an error occurred
	 */

	int recv(void* buffer, int bytes);

	/** Checks the amount of data currently available on the network
	 * 	@return The number of bytes available, -1 if network is not connected or error
	 */

	int isData();

	/** Whether or not the network is currently connected
	 *
	 */

	bool isConnected();

	/** Returns the socket used for the network connection
	 *
	 */

	int getSocket();



};



#endif /* NETWORKMANAGER_H_ */
