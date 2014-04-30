/*
 * EventNetwork.cpp
 *
 *  Created on: Apr 30, 2014
 *      Author: mitchell
 */

#include "EventNetwork.h"
#include "NetworkManager.h"

EventNetwork::EventNetwork() {
	//woo noething to do here folks

}

EventNetwork::~EventNetwork() {
	// TODO Auto-generated destructor stub
}


EventNetwork::EventNetwork(message_header header, std::string data) {
	setData(data);
	setObjectType(header.object_type);
	setLength(header.len);
	setOperation(parseMessageOp(header.op));
}



/** Returns the length of the data stored
 *
 */

int EventNetwork::getLength() {
	return length;
}

/** Returns the operation of the message
 *
 */

EventNetwork::MessageOp EventNetwork::getOperation() {
	return operation;
}

/** Returns the object type
 *
 */

std::string EventNetwork::getObjectType() {
	return objectType;
}

/** Returns the data of the message
 *
 */

std::string EventNetwork::getData() {
	return data;
}

/** Sets the length to the specified length
 *
 */

void EventNetwork::setLength(int len) {
	this->length = len;
}

/** Sets the message operation to the specified op
 *
 */

void EventNetwork::setOperation(MessageOp op) {
	this->operation = op;
}

/** Sets the object type to the specified type
 *
 */

void EventNetwork::setObjectType(std::string type) {
	this->objectType = type;
}

/** Sets the network data to the specified data
 *
 */

void EventNetwork::setData(std::string data) {
	this->data = data;
}

/** Converts the integer operation from the message header, into a MessageOp
 *
 */

EventNetwork::MessageOp EventNetwork::parseMessageOp(int op) {
	switch (op) {
	case MSG_OP_CREATE: return CREATE;
	case MSG_OP_UPDATE: return UPDATE;
	case MSG_OP_DELETE: return DELETE;
	case MSG_OP_KEYSTK: return KEYSTK;
	}
}
