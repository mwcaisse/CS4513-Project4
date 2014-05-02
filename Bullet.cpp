//
// Bullet.cpp
//

#include "Bullet.h"
#include "LogManager.h"
#include "WorldManager.h"
#include "ResourceManager.h"
#include "EventOut.h"
#include "Saucer.h"
#include "HostStatus.h"
#include "NetworkManager.h"

Bullet::Bullet(std::string serialized) {
	deserialize(serialized);
}

Bullet::Bullet(Position hero_pos) {

  // link to "bullet" sprite
  ResourceManager &resource_manager = ResourceManager::getInstance();
  Sprite *p_temp_sprite = resource_manager.getSprite("bullet");
  if (!p_temp_sprite) {
    LogManager &log_manager = LogManager::getInstance();
    log_manager.writeLog("Bullet::Bullet(): Warning! Sprite '%s' not found", "bullet");
  } else {
    setSprite(p_temp_sprite);
    setSpriteSlowdown(5);		
  }

  // set other object properties
  setType("Bullet");
  setXVelocity(1);		// move 1 space right every frame

  // set starting location, based on the hero's position passed in
  Position pos(hero_pos.getX()+3, hero_pos.getY());
  setPosition(pos);

  if (HostStatus::isHost()) {
 	  if (NetworkManager::getInstance().sendMessage(CREATE, getType(), getId(), serialize())) {
 		  LogManager& logger = LogManager::getInstance();
 		  logger.writeLog("Bullet::Bullet(): Unable to send create message to client");
 	  }
   }
}

// handle event
// return 0 if ignored, else 1
int Bullet::eventHandler(Event *p_e) {
	if (HostStatus::isHost()) { // only handle events if we are on the host
		if (p_e->getType() == OUT_EVENT) {
			out();
			return 1;
		}

		if (p_e->getType() == COLLISION_EVENT) {
			EventCollision *p_collision_event = static_cast <EventCollision *> (p_e);
			hit(p_collision_event);
			return 1;
		}
	}

		// if we get here, we have ignored this event
		return 0;
}

// if bullet moves outside world, mark self for deletion
void Bullet::out() {
  WorldManager &world_manager = WorldManager::getInstance();
  world_manager.markForDelete(this);
}

// if bullet hits saucer, mark saucer and bullet for deletion
void Bullet::hit(EventCollision *p_c) {
  WorldManager &world_manager = WorldManager::getInstance();
  world_manager.markForDelete(p_c->getObject1());
  world_manager.markForDelete(p_c->getObject2());
}


