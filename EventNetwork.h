/*
 * EventNetwork.h
 *
 *  Created on: Apr 30, 2014
 *      Author: mitchell
 */

#ifndef EVENTNETWORK_H_
#define EVENTNETWORK_H_

#include "Event.h"
#include "NetworkManager.h"

#define NETWORK_EVENT "__network_event__"

class EventNetwork: public Event {


private:

	/** The length of the network data */
	int length;
	/** The operation to perform */
	MessageOp operation;
	/** The type of object if applicable */
	std::string objectType;
	/** The id of the object, if applicable */
	int objectId;
	/** The data contained in the message */
	std::string data;

public:


	EventNetwork();

	EventNetwork(message_header header, std::string data);

	virtual ~EventNetwork();

	/** Returns the length of the data stored
	 *
	 */

	int getLength();

	/** Returns the operation of the message
	 *
	 */

	MessageOp getOperation();

	/** Returns the object type
	 *
	 */

	std::string getObjectType();

	/** Returns the object id
	 *
	 */

	int getObjectId();

	/** Returns the data of the message
	 *
	 */

	std::string getData();

	/** Sets the length to the specified length
	 *
	 */

	void setLength(int len);

	/** Sets the message operation to the specified op
	 *
	 */

	void setOperation(MessageOp op);

	/** Sets the object type to the specified type
	 *
	 */

	void setObjectType(std::string type);

	/** Sets the object id to the specified id
	 *
	 */

	void setObjectId(int id);

	/** Sets the network data to the specified data
	 *
	 */

	void setData(std::string data);

};

#endif /* EVENTNETWORK_H_ */
