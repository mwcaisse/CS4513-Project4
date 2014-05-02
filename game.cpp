//
// game.cpp
// 

#include <unistd.h>

// Engine includes
#include "GameManager.h"
#include "LogManager.h"
#include "Pause.h"
#include "ResourceManager.h"

// Game includes
#include "Saucer.h"
#include "Hero.h"
#include "Star.h"
#include "GameStart.h"

// Function prototypes
void loadResources(void);
void populateWorld(void);

int print_usage() {
	printf("Usage: \n");
	printf("\t game -h hostaddress -p port \n");
	printf("\t game -p port \n");
}

int main(int argc, char *argv[]) {

	//parse command line arguments

	char c;

	bool host = true;
	std::string hostAddress;
	std::string port;

	while ( (c = getopt(argc, argv, "h:p:")) != EOF) {
		switch (c) {

		case 'h':
			hostAddress = optarg;
			host = false;

			break;
		case 'p':
			//set the port
			port = optarg;
			break;
		default:
			print_usage();
			return 0;
		}
	}




  LogManager &log_manager = LogManager::getInstance();

  if (host) {
	  log_manager.writeLog("Game: Starting as client, connecting to %s \n", hostAddress.c_str(), port.c_str());
  }
  else {
	  log_manager.writeLog("Game: Starting a host \n");
  }

  // Start up Game Manager
  GameManager &game_manager = GameManager::getInstance();
  if (game_manager.startUp())  {
    log_manager.writeLog("Error starting game manager!");
    game_manager.shutDown();
    return 0;
  }

  //lets start the network?

  // Show splash screen
  splash();

  // Load game resources
  loadResources();

  // Setup some objects
  populateWorld();

  // Enable pausing
  new Pause;

  // Run the game (this blocks until the game loop is over)
  game_manager.run();

  // Shut everything down
  game_manager.shutDown();
}

// load resources (sprites, here)
void loadResources(void) {
  ResourceManager &resource_manager = ResourceManager::getInstance();
  resource_manager.loadSprite("sprites/saucer-spr.txt", "saucer");
  resource_manager.loadSprite("sprites/ship-spr.txt", "ship");
  resource_manager.loadSprite("sprites/bullet-spr.txt", "bullet");
  resource_manager.loadSprite("sprites/explosion-spr.txt", "explosion");
  resource_manager.loadSprite("sprites/gamestart-spr.txt", "gamestart");
  resource_manager.loadSprite("sprites/gameover-spr.txt", "gameover");
}

// populate world with some objects
void populateWorld(void) {

  // spawn some Stars
  for (int i=0; i<16; i++) 
    new Star;

  // create the GameStart object
  new GameStart();
}

