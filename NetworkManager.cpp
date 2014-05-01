/*
 * NetworkManager.cpp
 *
 *  Created on: Apr 30, 2014
 *      Author: mitchell
 */

#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/ioctl.h>

#include "NetworkManager.h"
#include "EventNetwork.h"

NetworkManager* NetworkManager::_instance = NULL;

NetworkManager::NetworkManager() {
	//create a new network manager
	sock = -1;
}

/** Returns the singleton instance of the NetworkManager */
NetworkManager* NetworkManager::getInstance() {
	if (!_instance) {
		_instance = new NetworkManager();
	}
	return _instance;
}

/** Starts the network manager
 *
 */

int NetworkManager::startUp() {

}

/** Shuts down the network manager
 *
 */

void NetworkManager::shutDown() {
	if (isConnected()) {
		close();
	}
}

/** Checks to make sure the event type is valid
 *
 */

bool NetworkManager::isValid(std::string event_type) {
	return event_type.compare(NETWORK_EVENT) == 0;
}

/** Listens for connections from the client
 *
 *	@param port The port to listen on
 *	@return 0 if success -1 otherwise
 */

int NetworkManager::accept(std::string port) {
	int serverSock = createServerSocket(port);
	if (serverSock == -1) {
		return -1;
	}

	//now lets accept
	sock = ::accept(serverSock, NULL, NULL);

	//check if it failed or not
	if (sock == -1) {
		::close(serverSock);
		return -1; //failed
	}

	//successful, close the server socket, and return success
	::close(serverSock);

	return 0;
}

/** Connects to the host at the specified address and port
 *
 * @param host The address of the host
 * @param port The port to connect to
 */

int NetworkManager::connect(std::string host, std::string port) {
	struct addrinfo hints;
	struct addrinfo* host_info;

	memset(&hints, 0, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	int res = getaddrinfo(host.c_str(), port.c_str(), &hints, &host_info);

	if (res) {
		//failed to get address information
		return -1;
	}

	sock = socket(host_info->ai_family, host_info->ai_socktype, host_info->ai_protocol);

	if (sock == -1) {
		//failed to create the socket
		return -1;
	}

	res = ::connect(sock, host_info->ai_addr, host_info->ai_addrlen);

	if (res == -1) {
		//failed to connect
		return -1;
	}

	//woo completed successfully, return success
	return 0;
}

/** Close the network connection
 *
 *	@return 0 if sucessful -1 otherwise
 */

int NetworkManager::close() {
	if (isConnected()) {
		::close(sock);
	}
	return 0;
}

/** Sends the specified message over the network
	 *  @param op The message Operation
	 *  @param objectType the object type of the message
	 *  @param the data in the message
	 *  @return The number of bytes sent, or -1 if error occurred
	 */

int NetworkManager::sendMessage(MessageOp op, std::string objectType, std::string data) {
	if (objectType.length() >= OBJECT_TYPE_LEN) {
		return -1;
	}

	message_header header;
	header.op = op;
	header.len = data.length() + 1; // + 1 for null terminator
	strncpy(header.object_type, objectType.c_str(), OBJECT_TYPE_LEN);

	int length = data.length() + sizeof(header);
	char buffer[length + 1];

	memcpy(buffer, (void*) &header, sizeof(header));
	memcpy(buffer + sizeof(header), data.c_str(), data.length() + 1);

	return send(buffer, length);

}

/** Sends the specified bytes over the connected network
 *
 * @param buffer The bytes to send
 * @param bytes The number of bytes to send
 * @return The number of bytes sent or -1 if error occurred
 *
 */

int NetworkManager::send(void* buffer, int bytes) {
	return ::send(sock, buffer, bytes, 0);
}

/** Retrieves a message from the network, will return the message in EventNetwork form
 * 		if a message exists. Will not do anything if a message isn't available
 *
 * 	@return The EventNetwork representing the message, NULL if no message available,
 * 		or an error occurred.
 */

EventNetwork* NetworkManager::recvMessage() {
	if (!isMessage()) {
		return NULL; // no message available, return
	}

	//the message header
	message_header header;
	int read = recv(&header, sizeof(header), false);

	if (read != sizeof(header)) {
		return NULL; // error header was not the right size
	}

	int dataLength = header.len;

	char buffer[dataLength];

	read = recv(buffer, dataLength, false);

	if (read == -1) {
		//error occurred while reading the message body
		return NULL;
	}

	std::string body = buffer;

	return new EventNetwork(header, body);

}

/** Reads up to the specified number of bytes into the buffer
 *
 * @param buffer The buffer to copy the data into
 * @param bytes The maximum number of bytes to read
 * @param peek If true will just peek at the data available.
 * @return The number of bytes read, -1 if an error occurred
 */

int NetworkManager::recv(void* buffer, int bytes, bool peek) {

	int flag = 0;

	if (peek) { //if peek, set the peek flag
		flag = MSG_PEEK;
	}

	return ::recv(sock, buffer, bytes, flag);
}

/** Checks the amount of data currently available on the network
 * 	@return The number of bytes available, -1 if network is not connected or error
 */

int NetworkManager::isData() {
	if (!isConnected()) {
		return -1; // we are not connected return failure
	}

	int bytes;
	int res = ioctl(sock, FIONREAD, &bytes);

	if (res == -1) {
		return res;
	}

	return bytes;
}

/** Determines if a full message (header + body) is available to be read from the network
 * 	@return True if a message is available, false otherwise.
 */

bool NetworkManager::isMessage() {
	int size = sizeof(message_header);
	if (isData() < size) {
		return false; // no message header available.
	}
	//message header is available, check if the full body is
	message_header header;
	int res = recv(&header, size, true); // peek at the header
	if (res < 0) {
		//recv failed, or isData lied?? the horror
		return false;
	}

	//add the body length to the header
	size += header.len;

	//check if the data available contains the body
	return isData() < size;

}

/** Whether or not the network is currently connected
 *
 */

bool NetworkManager::isConnected() {
	return sock > 0;
}

/** Returns the socket used for the network connection
 *
 */

int NetworkManager::getSocket() {
	return sock;
}

/** Creates the server socket for listening for new clients
* @param port The port to create the server socket on
* @return the created server socket or -1 if error
*/

int NetworkManager::createServerSocket(std::string port) {
	struct addrinfo hints;
	struct addrinfo* server_info;
	struct addrinfo* server_connect;

	int res = 0;
	int sock = -1;

	memset(&hints, 0, sizeof hints);

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	res = getaddrinfo(NULL, port.c_str(), &hints, &server_info);

	if (res) {
		std::cout << "Error occurred while retrieving our address info \n";
	}

	for (server_connect = server_info; server_connect != NULL; server_connect = server_connect->ai_next) {
		sock = socket(server_connect->ai_family, server_connect->ai_socktype, server_connect->ai_protocol);

		if (sock == -1) {
			//cannot bind to this address, try the next one
			continue;
		}

		int yes = 1;
		res = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

		if (res == -1) {
			//couldnt set sockopt
			::close(sock);
			sock = -1;
			continue;
		}

		res = bind(sock, server_connect->ai_addr, server_connect->ai_addrlen);

		if (res == -1) {
			::close(sock);
			sock = -1;
			//error binding
			continue;
		}
		break;
	}

	free(server_info);

	res = listen(sock, 0);

	if (res) {
		::close(sock);
		return -1;
	}

	return sock;

}
