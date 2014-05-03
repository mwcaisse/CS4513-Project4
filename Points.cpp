//
// Points.cpp
//

// game includes
#include "Points.h"

// engine includes
#include "Event.h"
#include "EventStep.h"
#include "GraphicsManager.h"
#include "LogManager.h"
#include "NetworkManager.h"
#include "HostStatus.h"


Points::Points() {
  setLocation(TOP_RIGHT);
  setViewString(POINTS_STRING);
  setColor(COLOR_YELLOW);
  // need to update score each second, so count "step" events
  registerInterest(STEP_EVENT);
}

// handle event
// return 0 if ignored, else 1
int Points::eventHandler(Event *p_e) {

  // Parent handles event if score update
  if (ViewObject::eventHandler(p_e)) {
    return 1;
  }

  // If step, increment score every second (30 steps)
  if (p_e->getType() == STEP_EVENT) {
    if (static_cast <EventStep *> (p_e) -> getStepCount() % 30 == 0) {
      setValue(getValue() + 1);
    }
    //send score update to server every 5 secconds if we are host
    if (HostStatus::isHost() && (static_cast <EventStep *> (p_e) -> getStepCount() % (30 * 5) == 0)) {
    	if (NetworkManager::getInstance().sendPointMessage(getValue())) {
    		LogManager::getInstance().writeLog("Points::eventHandler(): Failed to send points sync message to client");
    	}
    }
    return 1;
  }

  // if we get here, we have ignored this event
  return 0;
}

