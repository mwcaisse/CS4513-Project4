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
	//do some start up stuff
}

/** Shuts down the network manager
 *
 */

void NetworkManager::shutDown() {
	//TODO;
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

/** Sends the specified bytes over the connected network
 *
 * @param buffer The bytes to send
 * @param bytes The number of bytes to send
 * @return The number of bytes sent or -1 if error occurred
 *
 */

int NetworkManager::send(void* buffer, int bytes) {
	//TODO;
	return 1;
}

/** Reads up to the specified number of bytes into the buffer
 *
 * @param buffer The buffer to copy the data into
 * @param bytes The maximum number of bytes to read
 * @return The number of bytes read, -1 if an error occurred
 */

int NetworkManager::recv(void* buffer, int bytes) {
	//TODO;
	return 1;
}

/** Checks the amount of data currently available on the network
 * 	@return The number of bytes available, -1 if network is not connected or error
 */

int NetworkManager::isData() {
	//TODO;
	return 0;
}

/** Whether or not the network is currently connected
 *
 */

bool NetworkManager::isConnected() {
	return sock != -1;
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
