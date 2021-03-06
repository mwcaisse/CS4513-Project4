/*
 * NetworkManager.h
 *
 *  Created on: Apr 30, 2014
 *      Author: mitchell
 */

#ifndef NETWORKMANAGER_H_
#define NETWORKMANAGER_H_

#include "Manager.h"
#include "NetworkStats.h"


#define DRAGONFLY_PORT "9876"

#define OBJECT_TYPE_LEN 30

enum MessageOp {CREATE = 0, UPDATE = 1, DELETE = 2, KEYSTK = 3, GAME_OVER = 4, POINTS = 5, GAME_START = 6, QUIT = 7, TIME = 8};

struct _network_message_header {
	int op; // The operation this message is performing
	int len; // the length of the message
	int misc; // optional int field
	char object_type[OBJECT_TYPE_LEN]; // the object type, if applicable
};

typedef struct _network_message_header message_header;

class EventNetwork;
class NetworkSentry;

class NetworkManager : public Manager {


private:

	/** The network stats */
	NetworkStats* networkStats;

	/** Whether or not we are keep statistics */
	bool statistics;

	NetworkManager();

	NetworkManager(NetworkManager const&);

	void operator=(NetworkManager const&); // dont allow assignment

	/** The socket descriptor for the socket */
	int sock;

	/** The singleton instance */
	static NetworkManager _instance;

	/** The network sentry for the network manager */
	NetworkSentry* sentry;

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
	 * @param peek If true will just peek at the data available.
	 * @return The number of bytes read, -1 if an error occurred
	 */

	int recv(void* buffer, int bytes, bool peek);

public:

	/** Returns the singleton instance of the NetworkManager */
	static NetworkManager& getInstance();

	/** Starts the network manager
	 *
	 */

	int startUp(bool statistics = false);

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
	 *  @param misc The misc int in the header
	 *  @param body The message body
	 *  @return The number of bytes sent, or -1 if error occurred
	 */

	int sendMessage(MessageOp op, std::string objectType, int misc, std::string body = "");

	/** Helper function to send a new Object Create message
	 *  @param obj The object to create
	 */

	int sendCreateMessage(Object* obj);

	/** Helper function to send a new Object Update message
	 * 	@param obj The object to update
	 */

	int sendUpdateMessage(Object* obj);

	/** Helper function to send a new Object Delete message
	 *  @param obj The object to delete
	 */

	int sendDeleteMessage(Object* obj);

	/** Helper function to send a new Key stroke message
	 *  @param key The key code of the key pressed
	 */

	int sendKeyMessage(int key);

	/** Helper function to send the current points value
	 * 	@param points The current points value
	 */

	int sendPointMessage(int points);

	/** Helper function to notify that the game is over
	 *
	 */

	int sendGameOverMessage();

	/** Helper function to notify that a client is ready to start, or the host is starting the game
	 *
	 */

	int sendGameStartMessage();

	/** Helper function to send the quit message
	 *
	 */

	int sendQuitMessage();

	/** Retrieves a message from the network, will return the message in EventNetwork form
	 * 		if a message exists. Will not do anything if a message isn't available
	 *
	 * 	@return The EventNetwork representing the message, NULL if no message available,
	 * 		or an error occurred.
	 */

	EventNetwork* recvMessage();

	/** Determines if a full message (header + body) is available to be read from the network
	 * 	@return True if a message is available, false otherwise.
	 */

	bool isMessage();

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
