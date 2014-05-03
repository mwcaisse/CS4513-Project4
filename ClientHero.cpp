//
// Hero.cpp
//

// engine includes
#include "EventStep.h"
#include "EventView.h"
#include "GraphicsManager.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"

// game includes
#include "Bullet.h"
#include "EventNuke.h"
#include "Explosion.h"
#include "GameOver.h"
#include "GameStart.h"
#include "ClientHero.h"
#include "HostStatus.h"
#include "NetworkManager.h"

ClientHero::ClientHero() {

  // link to "ship" sprite
  ResourceManager &resource_manager = ResourceManager::getInstance();
  LogManager &log_manager = LogManager::getInstance();
  Sprite *p_temp_sprite;
  p_temp_sprite = resource_manager.getSprite("cship");
  if (!p_temp_sprite) {
    log_manager.writeLog("Hero::Hero(): Warning! Sprite '%s' not found", "cship");
  } else {
    setSprite(p_temp_sprite);
    setSpriteSlowdown(3);		  // third speed animation
    setTransparency();			  // transparent sprite
  }

  setType("Hero");

  if (HostStatus::isHost()) {
	  registerInterest(STEP_EVENT);
  }

  // set starting location
  WorldManager &world_manager = WorldManager::getInstance();
  Position pos(7, world_manager.getBoundary().getVertical()/4);
  setPosition(pos);

  fire_slowdown = 15;
  fire_countdown = fire_slowdown;

  nuke_count = 1;
  NetworkManager::getInstance().sendCreateMessage(this);

}

ClientHero::ClientHero(std::string serialized) {
	deserialize(serialized);
}


ClientHero::~ClientHero() {

	bool gameOverExists = false;

	ObjectList objectList = WorldManager::getInstance().getAllObjects();
	ObjectListIterator itr(&objectList);

	for (itr.first(); !itr.isDone(); itr.next()) {
		Object* obj = itr.currentObject();
		if (obj->getType() == "GameOver") {
			gameOverExists = true;
			break;
		}
	}

	if (!gameOverExists) {
	  // create GameOver object
	  GameOver *p_go = new GameOver;

	  // make big explosion
	  for (int i=-8; i<=8; i+=5) {
		for (int j=-5; j<=5; j+=3) {
		  Position temp_pos = this->getPosition();
		  temp_pos.setX(this->getPosition().getX() + i);
		  temp_pos.setY(this->getPosition().getY() + j);
		  Explosion *p_explosion = new Explosion(temp_pos);
		}
	  }
	}
}

int ClientHero::eventHandler(Event *p_e) {

  if (p_e->getType() == STEP_EVENT) {
    step();
    return 1;
  }
  // if we get here, we have ignored this event
  return 0;
}

// call move (or do nothing) according to key pressed
void ClientHero::keyboard(int key) {
  WorldManager &world_manager = WorldManager::getInstance();
  LogManager& logManager = LogManager::getInstance();

  //logManager.writeLog("ClientHero::keyboard() Key: %d ", key);

  switch(key) {
  case KEY_UP:			// up arrow
    move(-1);
    break;
  case KEY_DOWN:		// down arrow
    move(+1);
    break;
  case ' ':			// fire
    fire();
    break;
  /*case 13:			// nuke! NO NUKES atm
    nuke();
    break;*/
  case 'q':			// quit
    world_manager.markForDelete(this);
    break;
  default:
	  logManager.writeLog("ClientHero::keyboard() Switch fell through");
	  break;
  };
  return;
}

// move up or down
void ClientHero::move(int dy) {
  WorldManager &world_manager = WorldManager::getInstance();
  Position new_pos(getPosition().getX(), getPosition().getY() + dy);

  // if stays on screen, allow move
  if ((new_pos.getY() > 3) &&
      (new_pos.getY() < world_manager.getBoundary().getVertical()))
    world_manager.moveObject(this, new_pos);

  //notify the client that we have moved
  NetworkManager::getInstance().sendUpdateMessage(this);

}

// fire a bullet
void ClientHero::fire() {
  if (fire_countdown > 0)
    return;
  fire_countdown = fire_slowdown;
  new Bullet(getPosition(), "cbullet");
}

// decrease fire restriction
void ClientHero::step() {
  fire_countdown--;
  if (fire_countdown < 0)
    fire_countdown = 0;
}


