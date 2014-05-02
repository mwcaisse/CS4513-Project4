/*
 * HostStatus.cpp
 *
 *  Created on: May 1, 2014
 *      Author: mitchell
 */

#include "HostStatus.h"
#include "Client.h"
#include "Host.h"

bool HostStatus::bHost = true;
Host* HostStatus::host;
Client* HostStatus::client;

HostStatus::HostStatus() {

}

HostStatus::~HostStatus() {

}

bool HostStatus::isHost() {
	return bHost;
}

void HostStatus::setHost(bool isHost) {
	bHost = isHost;
}

Host* HostStatus::getHost() {
	return host;
}

void HostStatus::setHost(Host* h) {
	host = h;
}

Client* HostStatus::getClient() {
	return client;
}

void HostStatus::setClient(Client* c) {
	client = c;
}
