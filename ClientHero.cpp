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

  // set starting location
  WorldManager &world_manager = WorldManager::getInstance();
  Position pos(7, world_manager.getBoundary().getVertical()/2);
  setPosition(pos);

  fire_slowdown = 15;
  fire_countdown = fire_slowdown;

  nuke_count = 1;
}

ClientHero::~ClientHero() {

  // create GameOver object
  GameOver *p_go = new GameOver;

  // make big explosion
  for (int i=-8; i<=8; i+=5) {
    for (int j=-5; j<=5; j+=3) {
      Position temp_pos = this->getPosition();
      temp_pos.setX(this->getPosition().getX() + i);
      temp_pos.setY(this->getPosition().getY() + j);
      Explosion *p_explosion = new Explosion;
      p_explosion -> setPosition(temp_pos);
    }
  }
}

// move up or down
void ClientHero::move(int dy) {
  WorldManager &world_manager = WorldManager::getInstance();
  Position new_pos(getPosition().getX(), getPosition().getY() + dy);

  // if stays on screen, allow move
  if ((new_pos.getY() > 3) && 
      (new_pos.getY() < world_manager.getBoundary().getVertical()))
    world_manager.moveObject(this, new_pos);
}


