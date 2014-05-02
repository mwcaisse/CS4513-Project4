/*
 * HostStatus.cpp
 *
 *  Created on: May 1, 2014
 *      Author: mitchell
 */

#include "HostStatus.h"

bool HostStatus::host = true;

HostStatus::HostStatus() {

}

HostStatus::~HostStatus() {

}

bool HostStatus::isHost() {
	return host;
}

void HostStatus::setHost(bool isHost) {
	host = isHost;
}
