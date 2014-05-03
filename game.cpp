//
// game.cpp
// 

#include <unistd.h>
#include <iostream>

// Engine includes
#include "GameManager.h"
#include "LogManager.h"
#include "Pause.h"
#include "ResourceManager.h"
#include "NetworkManager.h"

// Game includes
#include "Saucer.h"
#include "Hero.h"
#include "Star.h"
#include "GameStart.h"
#include "Client.h"
#include "Host.h"
#include "HostStatus.h"
#include "StaticIds.h"


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

	bool isHost = true;
	std::string hostAddress;
	std::string port;

	while ( (c = getopt(argc, argv, "h:p:")) != EOF) {
		switch (c) {

		case 'h':
			hostAddress = optarg;
			isHost = false;

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

	HostStatus::setHost(isHost);

	if (port.length() == 0) {
		port = DRAGONFLY_PORT;
	}

	//std::cout << "Yisss, Address: " << hostAddress << " port: " << port << std::endl;


  LogManager &log_manager = LogManager::getInstance();

  // Start up Game Manager
  GameManager &game_manager = GameManager::getInstance();
  if (game_manager.startUp())  {
    log_manager.writeLog("Error starting game manager!");
    game_manager.shutDown();
    return 0;
  }

	if (HostStatus::isHost()) {
		log_manager.writeLog("Game: Starting a host \n");
	}
	else {
		log_manager.writeLog("Game: Starting as client, connecting to %s \n", hostAddress.c_str(), port.c_str());
	}

  //lets start the network?
  NetworkManager& networkManager = NetworkManager::getInstance();

  networkManager.startUp();

  if (HostStatus::isHost()) {
	  log_manager.writeLog("Game: Waiting for a user to connect... \n");
	  if (networkManager.accept(port)) {
		  log_manager.writeLog("Game: Fatal Error listening for clients");
		  game_manager.shutDown();
		  return 0;
	  }
	  HostStatus::setHost(new Host());
  }
  else {
	  log_manager.writeLog("Game: Connecting to server... \n");
	  if (networkManager.connect(hostAddress, port)) {
		  log_manager.writeLog("Game: Fatal Error connecting to host");
		  game_manager.shutDown();
		  return 0;
	  }
	  HostStatus::setClient(new Client());
  }


  // Show splash screen
  splash();

  // Load game resources
  loadResources();

  // Setup some objects
  populateWorld();

  // Enable pausing
  //TODO: for now disable pausing
  //new Pause;

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
  resource_manager.loadSprite("sprites/cship-spr.txt", "cship");
  resource_manager.loadSprite("sprites/bullet-spr.txt", "bullet");
  resource_manager.loadSprite("sprites/cbullet-spr.txt", "cbullet");
  resource_manager.loadSprite("sprites/explosion-spr.txt", "explosion");
  resource_manager.loadSprite("sprites/gamestart-spr.txt", "gamestart");
  resource_manager.loadSprite("sprites/gameover-spr.txt", "gameover");
}

// populate world with some objects
void populateWorld(void) {
	//this should be the third argument, shoudln't have to sync.. but anyways.
	// create the GameStart object
	GameStart* gameStart = new GameStart();
	gameStart->setId(GAME_START_ID);

	if (HostStatus::isHost()) {
		// spawn some Stars
		for (int i=0; i<16; i++)
			new Star;
	}


}

