//
// Explosion.cpp
//

#include "Explosion.h"
#include "EventStep.h"
#include "GameManager.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"
#include "HostStatus.h"
#include "NetworkManager.h"

Explosion::Explosion(Position pos) {
  registerInterest(STEP_EVENT);

  // link to "explosion" sprite
  ResourceManager &resource_manager = ResourceManager::getInstance();
  Sprite *p_temp_sprite = resource_manager.getSprite("explosion");
  if (!p_temp_sprite) {
    LogManager &log_manager = LogManager::getInstance();
    log_manager.writeLog("Explosion::Explosion(): Warning! Sprite '%s' not found", 
		"explosion");
    return;
  }
  setSprite(p_temp_sprite);

  setType("Explosion");

  setPosition(pos);

  time_to_live =  getSprite()->getFrameCount();
  setSolidness(SPECTRAL);

  if (HostStatus::isHost()) {
	  if (NetworkManager::getInstance().sendCreateMessage(this) == -1) {
		  LogManager::getInstance().writeLog("Explosion::Explosion(): error sending create message to client");
	  }
  }

}

Explosion::Explosion(std::string serialized) {
	deserialize(serialized);
	time_to_live =  getSprite()->getFrameCount(); // set ttl
}

// handle event
// return 0 if ignored, else 1
int Explosion::eventHandler(Event *p_e) {

  if (p_e->getType() == STEP_EVENT) {
    step();
    return 1;
  }

  // if we get here, we have ignored this event
  return 0;
}

void Explosion::step() {
  time_to_live--;
  if (time_to_live <= 0){
    WorldManager &world_manager = WorldManager::getInstance();
    world_manager.markForDelete(this);
  }
}

