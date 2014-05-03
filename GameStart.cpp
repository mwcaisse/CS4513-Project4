//
// GameStart.cpp
//

// engine includes
#include "EventKeyboard.h"
#include "EventStep.h"
#include "GameManager.h"
#include "GraphicsManager.h"	// for COLOR_YELLOW
#include "LogManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"

// game includes
#include "GameStart.h"
#include "Hero.h"
#include "Points.h"
#include "Saucer.h"
#include "HostStatus.h"
#include "Host.h"
#include "StaticIds.h"

GameStart::GameStart() {
  setType("GameStart");

  // link to "message" sprite
  ResourceManager &resource_manager = ResourceManager::getInstance();
  Sprite *p_temp_sprite = resource_manager.getSprite("gamestart");
  if (!p_temp_sprite) {
    LogManager &log_manager = LogManager::getInstance();
    log_manager.writeLog("GameStart::GameStart(): Warning! Sprite 'gamestart' not found");
  } else {
    setSprite(p_temp_sprite);
    setSpriteSlowdown(15);		  
  }

  // put in center of screen
  setLocation(CENTER_CENTER);

  // register for "keyboard" event
  registerInterest(KEYBOARD_EVENT);
}

// handle event
// return 0 if ignored, else 1
int GameStart::eventHandler(Event *p_e) {
   GameManager &game_manager = GameManager::getInstance();

  // keyboard
  if (p_e->getType() == KEYBOARD_EVENT) {
    EventKeyboard *p_keyboard_event = (EventKeyboard *) p_e;
    switch (p_keyboard_event->getKey()) {
    case 'p': 			// play
      start();
      break;
    case 'q':			// quit
      game_manager.setGameOver();
      break;
    default:
      break;
    }
    return 1;
  }

  // if we get here, we have ignored this event
  return 0;
}

void GameStart::start() {
	LogManager &logManager = LogManager::getInstance();
	logManager.writeLog("GameStart::start(): Starting the game");
	if (HostStatus::isHost()) {
		// create hero
		new Hero;

		HostStatus::getHost()->startGame();

		// spawn some saucers to shoot
		for (int i=0; i<16; i++) {
			new Saucer;
		}

	}

  // setup heads-up display
  Points* points = new Points;			// points display
  points->setId(POINTS_ID);

  /* TODO: no nukes right now
  ViewObject *p_vo = new ViewObject; // used for nuke count
  p_vo->setLocation(TOP_LEFT);
  p_vo->setViewString("Nukes");
  p_vo->setValue(1);
  p_vo->setColor(COLOR_YELLOW);
  */

  // when the game starts, become inactive
  setActive(false);
}

// override default draw so as not to display "value"
void GameStart::draw() {
  Object::draw();
}
