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
#include "NetworkManager.h"

GameStart::GameStart() {
  setType("GameStart");
  status = NONE;
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

/** Starts the game
 *
 */

void GameStart::startGame() {
	LogManager &logManager = LogManager::getInstance();
	logManager.writeLog("GameStart::startGame(): Starting the game");

	if (HostStatus::isHost()) {

		//tell the client to start the game
		if (NetworkManager::getInstance().sendGameStartMessage() == -1) {
			LogManager::getInstance().writeLog("GameStart::startGame(): Couldn't tell the client to start game");
		}

		// create hero
		new Hero;

		HostStatus::getHost()->startGame();

		// spawn some saucers to shoot
		for (int i=0; i<16; i++) {
			new Saucer;
		}
	}

	// setup heads-up display
	Points* points = new Points;	// points display
	points->setId(POINTS_ID);

	// when the game starts, become inactive
	setActive(false);
}

/** Other party is now ready
 *
 */

void GameStart::startOther() {
	if (HostStatus::isHost()) {
		//client or OTHER is now ready
		if (status == NONE) {
			status = OTHER;
		}
		// we are both now ready, start the game
		else if (status == ME) {
			status = BOTH;
			startGame();
		}
	}
	else {
		//The host has started the game, we start if we are ready or not
		status = BOTH;
		startGame();
	}
}

/** We have pressed the game start key
 *
 */

void GameStart::start() {

	if (HostStatus::isHost()) {
		if (status == OTHER) {
			status = BOTH;
			//we pressed ready, and the other party is ready, start the game
			startGame();
		}
		else if (status == NONE) {
			//we pressed ready, and client wasn't ready, set status to me, and change sprite
			status = ME;
			//TODO: update the sprite
		}

	}
	else {
		//if status is NONE or OTHER, then we tell the host we are ready
		if (status == NONE || status == OTHER) {
			status = ME;
			if (NetworkManager::getInstance().sendGameStartMessage() == -1) {
				LogManager::getInstance().writeLog("GameStart::start(): Couldn't send ready message to host");
			}
			//TODO: update the sprite
		}

	}

}

// override default draw so as not to display "value"
void GameStart::draw() {
  Object::draw();
}
