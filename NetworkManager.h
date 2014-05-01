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

#define OBJECT_TYPE_LEN 20

enum MessageOp {CREATE = 0, UPDATE = 1, DELETE = 2, KEYSTK = 3};

struct _network_message_header {
	int op; // The operation this message is performing
	int len; // the length of the message
	int object_id; // the id of the object, if applicable
	char object_type[OBJECT_TYPE_LEN]; // the object type, if applicable
};

typedef struct _network_message_header message_header;

class EventNetwork;

class NetworkManager : public Manager {


private:

	NetworkManager();

	NetworkManager(NetworkManager const&);

	void operator=(NetworkManager const&); // dont allow assignment

	/** The socket descriptor for the socket */
	int sock;

	static NetworkManager* _instance;

	/** Creates the server socket for listening for new clients
	 *
	 * @param port The port to create the server socket on
	 * @return 0 if sucessful -1 if error
	 */

	int createServerSocket(std::string port);

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

public:

	/** Returns the singleton instance of the NetworkManager */
	static NetworkManager* getInstance();

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

	/** Sends the specified message over the network
	 *  @param op The message Operation
	 *  @param objectType the object type of the message
	 *  @param the data in the message
	 *  @return The number of bytes sent, or -1 if error occurred
	 */

	int sendMessage(MessageOp op, std::string objectType, std::string data);

	/** Receives a message from the network
	 * 	@return NetworkEvent representing the message received, NULL if there was an error,
	 * 		or no message available
	 */

	EventNetwork* recvMessage();

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
