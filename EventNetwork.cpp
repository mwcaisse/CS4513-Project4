/*
 * EventNetwork.cpp
 *
 *  Created on: Apr 30, 2014
 *      Author: mitchell
 */

#include "EventNetwork.h"
#include "NetworkManager.h"

EventNetwork::EventNetwork() {
	//woo nothing to do here folks

}

EventNetwork::~EventNetwork() {
	// TODO Auto-generated destructor stub
}


EventNetwork::EventNetwork(message_header header, std::string data) {
	setData(data);
	setObjectType(header.object_type);
	setObjectId(header.object_id);
	setLength(header.len);
	setOperation((MessageOp)header.op);
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

MessageOp EventNetwork::getOperation() {
	return operation;
}

/** Returns the object type
 *
 */

std::string EventNetwork::getObjectType() {
	return objectType;
}

/** Returns the object id
 *
 */

int EventNetwork::getObjectId() {
	return objectId;
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

/** Sets the objectId to the specified id
 *
 */

void EventNetwork::setObjectId(int objectId) {
	this->objectId = objectId;
}

/** Sets the network data to the specified data
 *
 */

void EventNetwork::setData(std::string data) {
	this->data = data;
}

